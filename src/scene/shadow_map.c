#include "shadow_map.h"
#include "util/memory.h"
#include "math/mathf.h"
#include "math/matrix.h"

u16 __attribute__((aligned(64))) shadow_map_buffer[SHADOW_MAP_WIDTH * SHADOW_MAP_HEIGHT];


void shadowMapRender(struct RenderState* renderState, struct PointLight* from, struct Transform* subjectTransform, Gfx* subject, float subjectRadius) {
    struct Transform lightPovTransform;
    lightPovTransform.position = from->position;
    lightPovTransform.scale = gOneVec;

    struct Vector3 offset;
    vector3Sub(&subjectTransform->position, &from->position, &offset);
    quatLook(&offset, &gUp, &lightPovTransform.rotation);

    struct Transform povInverse;
    transformInvert(&lightPovTransform, &povInverse);

    float distance = sqrtf(vector3MagSqrd(&offset));

    float nearPlane = distance - subjectRadius;

    if (nearPlane < 0.00001f) {
        return;
    }
    
    float projOffset = nearPlane * subjectRadius / sqrtf(distance * distance - subjectRadius * subjectRadius);

    float projMatrix[4][4];
    u16 perspNorm;
    matrixPerspective(projMatrix, &perspNorm, -projOffset, projOffset, projOffset, -projOffset, nearPlane, distance + subjectRadius);
}

#define DEBUG_X 32
#define DEBUG_Y 32

void shadowMapRenderDebug(struct RenderState* renderState) {
    gDPPipeSync(renderState->dl);
    gDPLoadTextureBlock(
        renderState->dl++,
        shadow_map_buffer,
        G_IM_FMT_RGBA,
        G_IM_SIZ_16b,
        SHADOW_MAP_WIDTH,
        SHADOW_MAP_HEIGHT,
        0,
        G_TX_CLAMP | G_TX_NOMIRROR,
        G_TX_CLAMP | G_TX_NOMIRROR,
        5, 5,
        0, 0
    );
    gDPSetRenderMode(renderState->dl++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCombineLERP(renderState->dl++, 0, 0, 0, TEXEL0, 0, 0, 0, 1, 0, 0, 0, TEXEL0, 0, 0, 0, 1);
    gSPTextureRectangle(
        renderState->dl++, 
        DEBUG_X << 2, DEBUG_Y << 2, 
        (DEBUG_X + SHADOW_MAP_WIDTH) << 2, (DEBUG_Y + SHADOW_MAP_HEIGHT) << 2, 
        G_TX_RENDERTILE, 
        0, 0, 
        1 << 10, 1 << 10
    );
}