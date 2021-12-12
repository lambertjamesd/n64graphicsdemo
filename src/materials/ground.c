#include "ground.h"

#define GROUND_COMBINE_MODE        0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT

Lights1 ground_light = gdSPDefLights1(16, 100, 64, 32, 200, 128, 0x28, 0x28, 0x28);
Lights1 ground_light_in_shadow = gdSPDefLights1(16, 100, 64, 4, 25, 16, 0x28, 0x28, 0x28);


Gfx ground_mat[] = {
    gsDPPipeSync(),
    gsSPSetGeometryMode(G_LIGHTING | G_SHADE),
    gsDPSetCombineMode(GROUND_COMBINE_MODE, GROUND_COMBINE_MODE),
    gsSPSetLights1(ground_light),
    gsSPEndDisplayList(),
};

Gfx ground_in_shadow_mat[] = {
    gsDPPipeSync(),
    gsSPSetGeometryMode(G_LIGHTING | G_SHADE),
    gsDPSetCombineMode(GROUND_COMBINE_MODE, GROUND_COMBINE_MODE),
    gsSPSetLights1(ground_light_in_shadow),
    gsSPEndDisplayList(),
};