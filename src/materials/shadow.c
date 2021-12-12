
#include "shadow.h"

#define SHADOW_COMBINE_MODE     0, 0, 0, PRIMITIVE, 0, 0, 0, PRIMITIVE

Gfx shadow_mat[] = {
    gsDPPipeSync(),
    gsDPSetCombineMode(SHADOW_COMBINE_MODE, SHADOW_COMBINE_MODE),
    gsDPSetPrimColor(255, 255, 128, 128, 128, 128),
    gsSPEndDisplayList(),
};