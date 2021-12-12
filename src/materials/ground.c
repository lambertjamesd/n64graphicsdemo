#include "ground.h"

#define GROUND_COMBINE_MODE        0, 0, 0, PRIMITIVE, 0, 0, 0, PRIMITIVE

Gfx ground_mat[] = {
    gsDPPipeSync(),
    gsDPSetCombineMode(GROUND_COMBINE_MODE, GROUND_COMBINE_MODE),
    gsDPSetPrimColor(255, 255, 32, 200, 128, 255),
    gsSPEndDisplayList(),
};