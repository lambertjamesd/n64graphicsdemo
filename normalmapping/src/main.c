#include <ultra64.h>
#include <sched.h>
#include "defs.h"

#include "util/rom.h"
#include "util/time.h"
#include "graphics/graphics.h"
#include "scene/scene.h"
#include "controls/controller.h"

#ifdef WITH_DEBUGGER
#include "../debugger/debugger.h"
#endif

OSPiHandle *gPiHandle;

static OSMesg           PiMessages[DMA_QUEUE_SIZE];
static OSMesgQueue      PiMessageQ;

u64    mainStack[STACKSIZEBYTES/sizeof(u64)];

OSThread initThread;
u64 initThreadStack[STACKSIZEBYTES/sizeof(u64)];

OSThread gameThread;
u64 gameThreadStack[STACKSIZEBYTES/sizeof(u64)];

OSMesgQueue      gfxFrameMsgQ;
static OSMesg           gfxFrameMsgBuf[MAX_FRAME_BUFFER_MESGS];
static OSScClient       gfxClient;

static OSSched scheduler;
u64            scheduleStack[OS_SC_STACKSIZE/8];
OSMesgQueue	*schedulerCommandQueue;

void initProc(void *arg);
void gameProc(void *arg);

void main(void *arg) {
    osInitialize();

    gPiHandle = osCartRomInit();

    osCreateThread(
        &initThread,
        1,
        initProc,
        arg,
        (void*)(initThreadStack + (STACKSIZEBYTES/sizeof(u64))),
        (OSPri)INIT_PRIORITY
    );

    osStartThread(&initThread);
}

void initProc(void *arg) {
    osCreatePiManager(
        (OSPri)OS_PRIORITY_PIMGR,
        &PiMessageQ,
        PiMessages,
        DMA_QUEUE_SIZE
    );

    osCreateThread(
        &gameThread,
        6,
        gameProc,
        0,
        gameThreadStack + (STACKSIZEBYTES/sizeof(u64)),
        (OSPri)GAME_PRIORITY
    );

    osStartThread(&gameThread);

    osSetThreadPri(NULL, 0);
    for(;;);
}

extern OSMesgQueue dmaMessageQ;

extern char _heapStart[];

struct Scene gScene;

void gameProc(void *arg) {
    u8 schedulerMode = OS_VI_NTSC_HPF1;

	switch (osTvType) {
		case 0: // PAL
			schedulerMode = OS_VI_PAL_LPF1;
			break;
		case 1: // NTSC
			schedulerMode = OS_VI_NTSC_LPF1;
			break;
		case 2: // MPAL
            schedulerMode = OS_VI_MPAL_LPF1;
			break;
	}

    osCreateScheduler(
        &scheduler,
        (void *)(scheduleStack + OS_SC_STACKSIZE/8),
        SCHEDULER_PRIORITY,
        schedulerMode,
        1
    );

    schedulerCommandQueue = osScGetCmdQ(&scheduler);

    osCreateMesgQueue(&gfxFrameMsgQ, gfxFrameMsgBuf, MAX_FRAME_BUFFER_MESGS);
    osScAddClient(&scheduler, &gfxClient, &gfxFrameMsgQ);

	osViSetSpecialFeatures(OS_VI_GAMMA_OFF |
			OS_VI_GAMMA_DITHER_OFF |
			OS_VI_DIVOT_OFF |
			OS_VI_DITHER_FILTER_OFF);

    u32 pendingGFX = 0;
    u32 drawBufferIndex = 0;

    u16* memoryEnd = graphicsLayoutScreenBuffers((u16*)PHYS_TO_K0(osMemSize));

    heapInit(_heapStart, memoryEnd);

    sceneInit(&gScene);
    controllersInit();

#ifdef WITH_DEBUGGER
    OSThread* debugThreads[2];
    debugThreads[0] = &gameThread;
    gdbInitDebugger(gPiHandle, &dmaMessageQ, debugThreads, 1);
#endif

    while (1) {
        OSScMsg *msg = NULL;
        osRecvMesg(&gfxFrameMsgQ, (OSMesg*)&msg, OS_MESG_BLOCK);
        
        switch (msg->type) {
            case (OS_SC_RETRACE_MSG):
                static int renderSkip = 1;

                if (pendingGFX < 2 && !renderSkip) {
                    graphicsCreateTask(&gGraphicsTasks[drawBufferIndex], (GraphicsCallback)sceneRender, &gScene);
                    drawBufferIndex = drawBufferIndex ^ 1;
                    ++pendingGFX;
                } else if (renderSkip) {
                    --renderSkip;
                }

                controllersTriggerRead();
                sceneUpdate(&gScene);
                timeUpdateDelta();

                char msg[64];
                sprintf(msg, "current=%x start=%x end=%x dpstat=%x spstat=%x\n",
                    IO_READ(DPC_CURRENT_REG),					
                    IO_READ(DPC_START_REG),						
                    IO_READ(DPC_END_REG),						
                    IO_READ(DPC_STATUS_REG),					
                    IO_READ(SP_STATUS_REG));


                break;

            case (OS_SC_DONE_MSG):
                --pendingGFX;
                break;
            case (OS_SC_PRE_NMI_MSG):
                pendingGFX += 2;
                break;
            case SIMPLE_CONTROLLER_MSG:
                controllersUpdate();
                break;
        }
    }
}