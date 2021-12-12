#include <ultra64.h>

#define SHADOW_COMBINE_MODE        0, 0, 0, SHADE, 0, 0, 0, SHADE

// #define SHADOW_COMBINE_MODE        0, 0, 0, PRIMITIVE, 0, 0, 0, PRIMITIVE

Lights1 caster_light = gdSPDefLights1(16, 32, 100, 32, 64, 200, 0x28, 0x28, 0x28);

Gfx shadow_caster_mat[] = {
    gsDPPipeSync(),
    gsSPSetGeometryMode(G_LIGHTING | G_SHADE),
    gsDPSetPrimColor(255, 255, 32, 64, 200, 255),
    gsDPSetCombineMode(SHADOW_COMBINE_MODE, SHADOW_COMBINE_MODE),
    gsSPTexture(65535, 65535, 0, 0, 1),
    gsSPSetLights1(caster_light),
    gsSPEndDisplayList(),
};