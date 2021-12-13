#include "ground.h"

#define GROUND_COMBINE_MODE 0, 0, 0, PRIMITIVE, 0, 0, 0, ENVIRONMENT

Gfx ground_mat[] = {
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_LIGHTING | G_SHADE),
    gsDPSetPrimColor(255, 255, 32, 200, 128, 255),
    gsDPSetCombineMode(GROUND_COMBINE_MODE, GROUND_COMBINE_MODE),
    gsSPEndDisplayList(),
};