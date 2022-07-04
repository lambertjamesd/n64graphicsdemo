#include "basic_lit.h"

#define LIT_RENDER_MODE     0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT

Gfx gBasicLitMaterial[] = {
    gsSPSetGeometryMode(G_LIGHTING | G_SHADE),
    gsDPSetEnvColor(255, 255, 255, 255),
    gsDPSetCombineMode(LIT_RENDER_MODE, LIT_RENDER_MODE),
    gsSPEndDisplayList(),
};