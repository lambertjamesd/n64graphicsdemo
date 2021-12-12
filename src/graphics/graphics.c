#include "graphics.h"
#include "initgfx.h"

struct GraphicsTask gGraphicsTasks[2];

extern OSMesgQueue  gfxFrameMsgQ;
extern OSMesgQueue	*schedulerCommandQueue;

u64* rdpOutput;
u64 __attribute__((aligned(16))) dram_stack[SP_DRAM_STACK_SIZE64 + 1];
u64 __attribute__((aligned(16))) gfxYieldBuf2[OS_YIELD_DATA_SIZE/sizeof(u64)];
u32 firsttime = 1;

u16 __attribute__((aligned(16))) zbuffer[SCREEN_HT * SCREEN_WD];

u16* graphicsLayoutScreenBuffers(u16* memoryEnd) {
    gGraphicsTasks[0].framebuffer = memoryEnd - SCREEN_WD * SCREEN_HT;
    gGraphicsTasks[1].framebuffer = gGraphicsTasks[0].framebuffer - SCREEN_WD * SCREEN_HT;

    gGraphicsTasks[0].msg.type = OS_SC_DONE_MSG;
    gGraphicsTasks[1].msg.type = OS_SC_DONE_MSG;

    rdpOutput = (u64*)(gGraphicsTasks[1].framebuffer - SP_DRAM_STACK_SIZE8  / sizeof(u16));
    return (u16*)rdpOutput;
}

void graphicsCreateTask(struct GraphicsTask* targetTask, GraphicsCallback callback, void* data) {
    struct RenderState *renderState = &targetTask->renderState;

    renderStateInit(renderState);
    gSPSegment(renderState->dl++, 0, 0);

    gSPDisplayList(renderState->dl++, setup_rspstate);
    if (firsttime) {
        gSPDisplayList(renderState->dl++, rdpstateinit_dl);
	    firsttime = 0;
    }
    gSPDisplayList(renderState->dl++, setup_rdpstate);

    gDPSetDepthImage(renderState->dl++, osVirtualToPhysical(zbuffer));
    gDPPipeSync(renderState->dl++);
    gDPSetCycleType(renderState->dl++, G_CYC_FILL);
    gDPSetColorImage(renderState->dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, osVirtualToPhysical(zbuffer));
    gDPSetFillColor(renderState->dl++, (GPACK_ZDZ(G_MAXFBZ,0) << 16 | GPACK_ZDZ(G_MAXFBZ,0)));
    gDPFillRectangle(renderState->dl++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
	
    gDPPipeSync(renderState->dl++);
    gDPSetColorImage(renderState->dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, osVirtualToPhysical(targetTask->framebuffer));
    /* clearcolor is 32-bits (2 pixels wide) because fill mode
     * does two pixels at a time.
     */
    gDPSetFillColor(renderState->dl++, (GPACK_RGBA5551(255, 0, 0, 1) << 16 | 
			       GPACK_RGBA5551(255, 0, 0, 1)));
    gDPFillRectangle(renderState->dl++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
    gDPPipeSync(renderState->dl++);
    gDPSetCycleType(renderState->dl++, G_CYC_1CYCLE); 

    if (callback) {
        callback(data, renderState);
    }

    gDPFullSync(renderState->dl++);
    gSPEndDisplayList(renderState->dl++);

    renderStateFlushCache(renderState);

    OSScTask *scTask = &targetTask->task;

    OSTask_t *task = &scTask->list.t;

    task->data_ptr = (u64*)renderState->glist;
    task->data_size = (s32)renderState->dl - (s32)renderState->glist;
    task->type = M_GFXTASK;
    task->flags = OS_TASK_LOADABLE;
    task->ucode_boot = (u64*)rspbootTextStart;
    task->ucode_boot_size = (u32)rspbootTextEnd - (u32)rspbootTextStart;
    task->ucode = (u64*)gspF3DEX2_fifoTextStart;
    task->ucode_data = (u64*)gspF3DEX2_fifoDataStart;
    task->output_buff = (u64*)rdpOutput;
    task->output_buff_size = ((u64*)rdpOutput + SP_DRAM_STACK_SIZE64);
    task->ucode_data_size = SP_UCODE_DATA_SIZE;
    task->dram_stack = (u64*)dram_stack;
    task->dram_stack_size = SP_DRAM_STACK_SIZE8;
    task->yield_data_ptr = (u64*)gfxYieldBuf2;
    task->yield_data_size = OS_YIELD_DATA_SIZE;

    scTask->flags = 
        OS_SC_NEEDS_RSP | 
        OS_SC_NEEDS_RDP | 
        OS_SC_LAST_TASK |
		OS_SC_SWAPBUFFER;
    scTask->framebuffer = targetTask->framebuffer;
    scTask->msg = &targetTask->msg;
    scTask->msgQ = &gfxFrameMsgQ;
    scTask->next = 0;
    scTask->state = 0;

    osSendMesg(schedulerCommandQueue, (OSMesg)scTask, OS_MESG_BLOCK);
}