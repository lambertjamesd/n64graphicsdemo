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
};

extern struct GraphicsTask gGraphicsTasks[2];

typedef void (*GraphicsCallback)(void* data, struct RenderState* renderState);

u16* graphicsLayoutScreenBuffers(u16* memoryEnd);
void graphicsCreateTask(struct GraphicsTask* targetTask, GraphicsCallback callback, void* data);

#endif