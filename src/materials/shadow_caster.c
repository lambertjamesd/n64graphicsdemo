#include <ultra64.h>

#define SHADOW_COMBINE_MODE        0, 0, 0, SHADE, 0, 0, 0, SHADE

Lights1 caster_light = gdSPDefLights1(16, 32, 100, 32, 64, 200, 0x28, 0x28, 0x28);

Gfx shadow_caster_mat[] = {
    gsDPPipeSync(),
    gsSPSetGeometryMode(G_LIGHTING | G_SHADE),
    gsDPSetCombineMode(SHADOW_COMBINE_MODE, SHADOW_COMBINE_MODE),
    gsSPSetLights1(caster_light),
    gsSPEndDisplayList(),
};