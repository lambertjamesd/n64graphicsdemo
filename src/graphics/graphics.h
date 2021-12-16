#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <ultra64.h>
#include <sched.h>
#include "renderstate.h"

#define SCREEN_WD   640
#define SCREEN_HT   480

struct GraphicsTask {
    struct RenderState renderState;
    OSScTask task;
    OSScMsg msg;
    u16 *framebuffer;
    u16 taskIndex;
};

extern struct GraphicsTask gGraphicsTasks[2];
extern Vp fullscreenViewport;

#define GET_GFX_TYPE(gfx)       (_SHIFTR((gfx)->words.w0, 24, 8))

typedef void (*GraphicsCallback)(void* data, struct RenderState* renderState, struct GraphicsTask* task);

u16* graphicsLayoutScreenBuffers(u16* memoryEnd);
void graphicsCreateTask(struct GraphicsTask* targetTask, GraphicsCallback callback, void* data);

#endif