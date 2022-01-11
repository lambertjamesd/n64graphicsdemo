#include "scene.h"

#include "../graphics/copycb.h"

void sceneInit(struct Scene* scene) {
    for (unsigned i = 0; i < SCREEN_WD * SCREEN_HT; ++i) {
        indexColorBuffer[i] = (i * 256 / SCREEN_WD) % 256;
    }
}

void sceneUpdate(struct Scene* scene) {

}

void sceneRender(struct Scene* scene, struct RenderState* renderState, struct GraphicsTask* task) {
    // gDPPipeSync(renderState->dl++);
    // gDPSetColorImage(renderState->dl++, G_IM_FMT_CI, G_IM_SIZ_8b, SCREEN_WD, indexColorBuffer);
    // gDPSetCycleType(renderState->dl++, G_CYC_FILL);
    // gDPSetFillColor(renderState->dl++, 0);
    // gDPFillRectangle(renderState->dl++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);

    gDPPipeSync(renderState->dl++);
    gDPSetCycleType(renderState->dl++, G_CYC_1CYCLE);
    gDPSetColorImage(renderState->dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, osVirtualToPhysical(task->framebuffer));
    gSPSegment(renderState->dl++, SOURCE_CB_SEGMENT, indexColorBuffer);
    gSPDisplayList(renderState->dl++, gCopyCB);
}