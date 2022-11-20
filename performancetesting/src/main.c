
#include <ultra64.h>
#include <sched.h>

#include "defs.h"
#include "util/rom.h"
#include "util/time.h"
#include "util/memory.h"
#include "string.h"
#include "graphics/graph.h"

#include "display_list_tasks/tasks.h"
#include "display_list_tasks/base.h"

#ifdef WITH_DEBUGGER
#include "../debugger/debugger.h"
#endif

u16	__attribute__((aligned(64))) cfb_16_a[SCREEN_WD*SCREEN_HT];
u16 __attribute__((aligned(64))) cfb_16_b[SCREEN_WD*SCREEN_HT];

u16	*cfb_tbl[2] = {
  cfb_16_a, cfb_16_b
};

#define NUM_LINES 24
#define NUM_COLS  70

char textGrid[NUM_LINES][NUM_COLS + 1];
u8 nextLineIndex;
int frame;


static OSThread gameThread;
static OSThread initThread;

u64    mainStack[STACKSIZEBYTES/sizeof(u64)];
static u64 gameThreadStack[STACKSIZEBYTES/sizeof(u64)];
static u64 initThreadStack[STACKSIZEBYTES/sizeof(u64)];

static void gameProc(void *);
static void initProc(void *);

static OSMesg           PiMessages[DMA_QUEUE_SIZE];
static OSMesgQueue      PiMessageQ;

OSMesgQueue      gfxFrameMsgQ;
static OSMesg           gfxFrameMsgBuf[MAX_FRAME_BUFFER_MESGS];

OSMesgQueue     rdpMessageQ;

OSMesg          rdpMessageBuf;

OSPiHandle	*gPiHandle;

void main(void *arg) {
    osInitialize();

    gPiHandle = osCartRomInit();

    osCreateThread(
        &initThread, 
        1, 
        initProc, 
        NULL,
        (void *)(initThreadStack+(STACKSIZEBYTES/sizeof(u64))), 
		(OSPri)INIT_PRIORITY
    );

    osStartThread(&initThread);
}

static void initProc(void* arg) {
    osCreatePiManager(
        (OSPri) OS_PRIORITY_PIMGR, 
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

void println(char* text)
{
  char* nextLine = textGrid[nextLineIndex];
  char* lineEnd = nextLine + NUM_COLS;

  while (*text && nextLine < lineEnd)
  {
    *nextLine = *text;
    ++nextLine;
    ++text;
  }

  while (nextLine < lineEnd)
  {
    *nextLine = ' ';
    ++nextLine;
  }
  
  if (nextLineIndex == 0)
  {
    nextLineIndex = NUM_LINES - 1;
  }
  else
  {
    --nextLineIndex;
  }
}

void displayConsoleLog() {
    int line;

    for (line = 0; line < NUM_LINES; ++line)
    {
      printstr(white, 5, 2 + NUM_LINES - line, textGrid[(line + nextLineIndex) % NUM_LINES]);
    }

    osWritebackDCacheAll();
    osViSwapBuffer( cfb_tbl[frame] );
    frame ^= 1;
}

extern OSMesgQueue dmaMessageQ;

extern char _heapStart[];

Gfx glist[256];

#define RDP_OUTPUT_LEN	(4096*16)

u64 dram_stack[SP_DRAM_STACK_SIZE64] __attribute__((aligned (16))); /* used for matrix stack */
u64 rdp_output[RDP_OUTPUT_LEN] __attribute__((aligned (16))); /* buffer for RDP DL */


/*
 * Task header.
 */
OSTask          taskHeader = {
    .t = {
        M_GFXTASK,		/* type of task */
        OS_TASK_DP_WAIT,	/* flags - wait for DP to be available */
        NULL,			/* ucode boot (fill in later) */
        0,			/* ucode boot size (fill in later) */
        NULL,			/* ucode (fill in later) */
        SP_UCODE_SIZE,		/* ucode size */
        NULL,			/* ucode data (fill in later) (to init DMEM) */
        SP_UCODE_DATA_SIZE,	/* ucode data size */
        &dram_stack[0],		/* stack used by ucode */
        SP_DRAM_STACK_SIZE8,	/* size of stack */
        &rdp_output[0],		/* fifo output buffer start */
        &rdp_output[0]+RDP_OUTPUT_LEN,	/* fifo output buffer end */
        NULL,			/* display list pointer (fill in later) */
        0,			/* display list size (ignored) */
        NULL,			/* yield buffer (used if yield will occur) */
        0			/* yield buffer length */
    }
};

static void gameProc(void* arg) {
    u8 schedulerMode = OS_VI_NTSC_HPF1;

	switch (osTvType) {
		case 0: // PAL
			schedulerMode = OS_VI_PAL_HPF1;
			break;
		case 1: // NTSC
			schedulerMode = OS_VI_NTSC_HPF1;
			break;
		case 2: // MPAL
            schedulerMode = OS_VI_MPAL_HPF1;
			break;
	}

    osViSetMode(&osViModeTable[schedulerMode]);


	osViBlack(1);
	osViSetSpecialFeatures(OS_VI_GAMMA_OFF |
			OS_VI_GAMMA_DITHER_OFF |
			OS_VI_DIVOT_OFF |
			OS_VI_DITHER_FILTER_OFF);

    osCreateMesgQueue(&gfxFrameMsgQ, gfxFrameMsgBuf, MAX_FRAME_BUFFER_MESGS);

	osCreateMesgQueue(&rdpMessageQ, &rdpMessageBuf, 1);
	osSetEventMesg(OS_EVENT_DP, &rdpMessageQ, NULL);

    romInit();

#ifdef WITH_DEBUGGER
    OSThread* debugThreads[2];
    debugThreads[0] = &gameThread;
    gdbInitDebugger(gPiHandle, &dmaMessageQ, debugThreads, 1);
#endif

    int currentTask = 0;

    char printedMessage[64];

    while (1) {
        Gfx* dl = glist;

        gSPSegment(dl++, 0, 0);
        gSPSegment(dl++, TEST_TASK_SEGMENT, gTasks[currentTask].measuredTask);
        if (gTasks[currentTask].setupTask) {
            gSPDisplayList(dl++, gTasks[currentTask].setupTask);
        }
        gSPDisplayList(dl++, gRecursiveTest);

        gDPFullSync(dl++);
        gSPEndDisplayList(dl++);

        OSTask* theadp = &taskHeader;

        theadp->t.ucode_boot = (u64 *) rspbootTextStart;
        theadp->t.ucode_boot_size = 
                (u32) rspbootTextEnd - (u32) rspbootTextStart;

        theadp->t.ucode = (u64 *) gspF3DEX2_fifoTextStart;
        theadp->t.ucode_data = (u64 *) gspF3DEX2_fifoDataStart;

        theadp->t.data_ptr = (u64 *) glist;
        theadp->t.data_size = 
            (u32) ((dl - glist) * sizeof(Gfx));

        osWritebackDCache(&glist, sizeof(glist));
        u64 timeBefore = osGetTime();
        osSpTaskStart(theadp);
        (void)osRecvMesg(&rdpMessageQ, NULL, OS_MESG_BLOCK);
        u64 totalTime = osGetTime() - timeBefore;

        sprintf(printedMessage, "%s time = %d", gTasks[currentTask].name, (int)totalTime);

        println(printedMessage);

        displayConsoleLog();

        currentTask = (currentTask + 1) % gTaskCount;
    }
}