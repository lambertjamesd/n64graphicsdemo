#ifndef __COPYCB__H
#define __COPYCB__H

#include <ultra64.h>

#define SOURCE_CB_SEGMENT       0x8
#define SOURCE_COLOR_SEGMENT    0x9

#define SOURCE_CB       ((SOURCE_CB_SEGMENT) << 24)
#define SOURCE_COLOR    ((SOURCE_COLOR_SEGMENT) << 24)

extern Gfx gCombineBuffers[];
extern Gfx gAdjustBrightnessRange[];
extern u8 __attribute__((aligned(64))) indexColorBuffer[];
extern u8 __attribute__((aligned(64))) lightnessBuffer[];

extern u16 __attribute__((aligned(64))) colorBuffer[];

extern u64 gPalleteMapping[];

#endif