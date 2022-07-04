#include "outline_pass.h"

#define OUTLINE_LERP      0, 0, 0, ENVIRONMENT, 0, 0, 0, ENVIRONMENT

Gfx gOutlinePass[] = {
    gsDPPipeSync(),
    gsSPGeometryMode(G_CULL_BACK, G_CULL_FRONT),
    gsDPSetRenderMode(G_RM_ZB_OPA_DECAL, G_RM_ZB_OPA_DECAL2),
    gsDPSetCombineMode(OUTLINE_LERP, OUTLINE_LERP),
    gsDPSetEnvColor(0, 0, 0, 255),
    gsSPEndDisplayList(),
};