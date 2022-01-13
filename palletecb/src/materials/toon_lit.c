
#include "toon_lit.h"

#define TOON_LERP      SHADE, 0, ENVIRONMENT, PRIMITIVE, 0, 0, 0, ENVIRONMENT

void toonLitUse(struct RenderState* renderState, unsigned colorStart, unsigned colorWidth) {
    gDPPipeSync(renderState->dl++);
    gSPGeometryMode(renderState->dl++, G_CULL_FRONT, G_CULL_BACK | G_LIGHTING | G_SHADE);
    gDPSetCombineMode(renderState->dl++, TOON_LERP, TOON_LERP);
    gDPSetRenderMode(renderState->dl++, G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2);
    gDPSetEnvColor(renderState->dl++, (colorWidth - 1), (colorWidth - 1), (colorWidth - 1), 255);
    gDPSetPrimColor(renderState->dl++, 255, 255, colorStart, colorStart, colorStart, 255);
}