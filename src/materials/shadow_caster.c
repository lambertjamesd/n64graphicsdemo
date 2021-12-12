#include <ultra64.h>

#define SHADOW_COMBINE_MODE        0, 0, 0, PRIMITIVE, 0, 0, 0, PRIMITIVE

Gfx shadow_caster_mat[] = {
    gsDPPipeSync(),
    gsDPSetCombineMode(SHADOW_COMBINE_MODE, SHADOW_COMBINE_MODE),
    gsDPSetPrimColor(255, 255, 32, 64, 200, 255),
    gsSPEndDisplayList(),
};