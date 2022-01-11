#ifndef __COPYCB__H
#define __COPYCB__H

#include <ultra64.h>

#define SOURCE_CB_SEGMENT   0x8

#define SOURCE_CB   ((SOURCE_CB_SEGMENT) << 24)

extern Gfx gCopyCB[];
extern u8 __attribute__((aligned(64))) indexColorBuffer[];

#endif