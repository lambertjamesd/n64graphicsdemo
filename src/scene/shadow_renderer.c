
#include "shadow_renderer.h"
#include "materials/shadow.h"
#include "sk64/skelatool_defs.h"

void shadowRendererInit(struct ShadowRenderer* shadowRenderer, Gfx* gfx, unsigned casterBoneIndex, unsigned bottomShadowIndex, float shadowLength) {
    shadowRenderer->gfx = gfx;
    shadowRenderer->casterBoneIndex = casterBoneIndex;
    shadowRenderer->bottomShadowIndex = bottomShadowIndex;
    shadowRenderer->shadowLength = shadowLength;
    shadowRenderer->lightPosition = gUp;
    transformInitIdentity(&shadowRenderer->casterTransform);
}

void shadowRendererRender(struct ShadowRenderer* shadowRenderer, struct RenderState* renderState, struct ShadowReceiver* recievers, Mtx* recieverMatrices, unsigned recieverCount) {
    // first pass for shadowed objects
    for (unsigned i = 0; i < recieverCount; ++i) {
        gSPMatrix(renderState->dl++, &recieverMatrices[i], G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPDisplayList(renderState->dl++, recievers[i].litMaterial);
        gSPDisplayList(renderState->dl++, recievers[i].geometry);
        gSPPopMatrix(renderState->dl++, G_MTX_MODELVIEW);
    }

    // calculate position of top and bottom of shadow
    Mtx* shadowMatrices = renderStateRequestMatrices(renderState, 2);
    transformToMatrixL(&shadowRenderer->casterTransform, &shadowMatrices[shadowRenderer->casterBoneIndex]);

    struct Vector3 lightOffset;
    vector3Sub(&shadowRenderer->casterTransform.position, &shadowRenderer->lightPosition, &lightOffset);

    float lightDistance = sqrtf(vector3MagSqrd(&lightOffset));

    struct Vector3 shadowUp;
    quatMultVector(&shadowRenderer->casterTransform.rotation, &gUp, &shadowUp);
    float lightVerticalOffset = vector3Dot(&shadowUp, &lightOffset);

    struct Transform shadowEnd;
    shadowEnd.rotation = shadowRenderer->casterTransform.rotation;
    vector3AddScaled(
        &shadowRenderer->casterTransform.position, 
        &lightOffset, 
        shadowRenderer->shadowLength / lightDistance,
        &shadowEnd.position
    );
    vector3Scale(&gOneVec, &shadowEnd.scale, (lightDistance + shadowRenderer->shadowLength) / lightDistance);
    transformToMatrixL(&shadowEnd, &shadowMatrices[shadowRenderer->bottomShadowIndex]);

    // render back of shadows
    gDPPipeSync(renderState->dl++);
    // check if the shadow is inside out and need culling to flipped
    if (lightVerticalOffset < 0.0f) {
        gSPGeometryMode(renderState->dl++, G_CULL_BACK, G_CULL_FRONT);
    }
    gDPSetRenderMode(renderState->dl++, G_RM_ZB_XLU_SURF | Z_UPD, G_RM_ZB_XLU_SURF2 | Z_UPD);
    gSPSegment(renderState->dl++, MATRIX_TRANSFORM_SEGMENT, shadowMatrices);
    gSPDisplayList(renderState->dl++, shadow_mat);
    gSPDisplayList(renderState->dl++, shadowRenderer->gfx);


    gDPPipeSync(renderState->dl++);
    gDPSetRenderMode(renderState->dl++, G_RM_ZB_OPA_DECAL, G_RM_ZB_OPA_DECAL2);

    if (lightVerticalOffset < 0.0f) {
        gSPGeometryMode(renderState->dl++, G_CULL_FRONT, G_CULL_BACK);
    }
    // second pass for shadowed objects
    for (unsigned i = 0; i < recieverCount; ++i) {
        gSPMatrix(renderState->dl++, &recieverMatrices[i], G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPDisplayList(renderState->dl++, recievers[i].shadowMaterial);
        gSPDisplayList(renderState->dl++, recievers[i].geometry);
        gSPPopMatrix(renderState->dl++, G_MTX_MODELVIEW);
    }

    // render front of shadows
    gDPPipeSync(renderState->dl++);
    // check if the shadow is inside out and need culling to flipped
    if (lightVerticalOffset > 0.0f) {
        gSPGeometryMode(renderState->dl++, G_CULL_BACK, G_CULL_FRONT);
    }
    gDPSetRenderMode(renderState->dl++, G_RM_ZB_XLU_SURF | Z_UPD, G_RM_ZB_XLU_SURF2 | Z_UPD);
    gSPSegment(renderState->dl++, MATRIX_TRANSFORM_SEGMENT, shadowMatrices);
    gSPDisplayList(renderState->dl++, shadow_mat);
    gSPDisplayList(renderState->dl++, shadowRenderer->gfx);


    gDPPipeSync(renderState->dl++);
    gDPSetRenderMode(renderState->dl++, G_RM_ZB_OPA_DECAL, G_RM_ZB_OPA_DECAL2);
    if (lightVerticalOffset > 0.0f) {
        gSPGeometryMode(renderState->dl++, G_CULL_FRONT, G_CULL_BACK);
    }
    // third pass for shadowed objects
    for (unsigned i = 0; i < recieverCount; ++i) {
        gSPMatrix(renderState->dl++, &recieverMatrices[i], G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPDisplayList(renderState->dl++, recievers[i].litMaterial);
        gSPDisplayList(renderState->dl++, recievers[i].geometry);
        gSPPopMatrix(renderState->dl++, G_MTX_MODELVIEW);
    }
}