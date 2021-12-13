
#include "shadow_renderer.h"
#include "util/memory.h"
#include "defs.h"

#define MATRIX_TRANSFORM_SEGMENT    0xC
#define TOP_MATRIX_INDEX        0
#define BOTTOM_MATRIX_INDEX     1

#define SHADOW_COMBINE_MODE     0, 0, 0, PRIMITIVE, 0, 0, 0, PRIMITIVE

Gfx shadow_mat[] = {
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_LIGHTING),
    gsDPSetCombineMode(SHADOW_COMBINE_MODE, SHADOW_COMBINE_MODE),
    gsDPSetPrimColor(255, 255, 128, 128, 128, 0),
    gsSPEndDisplayList(),
};

void shadowRendererNewGfxFromOutline(struct Vector2* outline, unsigned pointCount, Gfx** gfxOut, Vtx** vtxOut) {
    Vtx* vtxResult = malloc(sizeof(Vtx) * pointCount * 2);
    Gfx* gfxResult = malloc(sizeof(Gfx) * (7 + pointCount + (pointCount - 1) / 2));

    for (unsigned i = 0; i < pointCount; ++i) {
        vtxResult[i].v.ob[0] = (short)outline[i].x;
        vtxResult[i].v.ob[1] = 0.0f;
        vtxResult[i].v.ob[2] = (short)outline[i].y;

        vtxResult[i + pointCount].v.ob[0] = (short)(SCENE_SCALE * outline[i].x);
        vtxResult[i + pointCount].v.ob[1] = 0.0f;
        vtxResult[i + pointCount].v.ob[2] = (short)(SCENE_SCALE * outline[i].y);
    }

    Gfx* gfxCurrent = gfxResult;

    gSPMatrix(gfxCurrent++, (Mtx*)(MATRIX_TRANSFORM_SEGMENT << 24) + TOP_MATRIX_INDEX, G_MTX_MODELVIEW | G_MTX_PUSH | G_MTX_MUL);
    gSPVertex(gfxCurrent++, vtxResult, pointCount, 0);
    gSPPopMatrix(gfxCurrent++, G_MTX_MODELVIEW);
    
    gSPMatrix(gfxCurrent++, (Mtx*)(MATRIX_TRANSFORM_SEGMENT << 24) + BOTTOM_MATRIX_INDEX, G_MTX_MODELVIEW | G_MTX_PUSH | G_MTX_MUL);
    gSPVertex(gfxCurrent++, vtxResult, pointCount, pointCount);
    gSPPopMatrix(gfxCurrent++, G_MTX_MODELVIEW);

    for (unsigned i = 0; i < pointCount; ++i) {
        unsigned curr = i;
        unsigned next = (i + 1) % pointCount;
        unsigned bottomCurr = i + pointCount;
        unsigned bottomNext = next + pointCount;
        gSP2Triangles(
            gfxCurrent++, 
            next, 
            curr, 
            bottomNext, 
            0, 
            bottomCurr, 
            bottomNext, 
            curr, 
            0
        );
    }

    for (unsigned i = 1; i + 1 < pointCount; i += 2) {
        if (i + 2 < pointCount) {
            gSP2Triangles(
                gfxCurrent++, 
                pointCount, 
                pointCount + i, 
                pointCount + i + 1,
                0,
                pointCount,
                pointCount + i + 1,
                pointCount + i + 2,
                0
            );
        } else {
            gSP1Triangle(
                gfxCurrent++,
                pointCount,
                pointCount + 1,
                pointCount + i + 1,
                0
            );
        }
    }

    gSPEndDisplayList(gfxCurrent++);

    *gfxOut = gfxResult;
    if (vtxOut) {
        *vtxOut = vtxResult;
    }
}

void shadowRendererInit(struct ShadowRenderer* shadowRenderer, struct Vector2* outline, unsigned pointCount, float shadowLength) {
    shadowRenderer->shadowLength = shadowLength;
    transformInitIdentity(&shadowRenderer->casterTransform);
    shadowRendererNewGfxFromOutline(
        outline, 
        pointCount, 
        &shadowRenderer->shape, 
        &shadowRenderer->vertices
    );
}

void shadowRendererRender(
    struct ShadowRenderer* shadowRenderer, 
    struct RenderState* renderState,
    struct PointLight* fromLight, 
    struct ShadowReceiver* recievers, 
    unsigned recieverCount
) {
    Mtx* recieverMatrices = renderStateRequestMatrices(renderState, recieverCount);
    unsigned lightCount = 0;

    for (unsigned i = 0; i < recieverCount; ++i) {
        struct ShadowReceiver* reciever = &recievers[i];
        transformToMatrixL(&reciever->transform, &recieverMatrices[i]);

        if (reciever->flags & ShadowReceiverFlagsUseLight) {
            ++lightCount;
        }
    }
    
    Light* lights = renderStateRequestLights(renderState, lightCount);
    
    unsigned currentLight = 0;

    // first pass for shadowed objects
    for (unsigned i = 0; i < recieverCount; ++i) {
        struct ShadowReceiver* reciever = &recievers[i];

        gSPMatrix(renderState->dl++, &recieverMatrices[i], G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPDisplayList(renderState->dl++, reciever->litMaterial);

        if (reciever->flags & ShadowReceiverFlagsUseLight) {
            Light* currLight = &lights[currentLight];
            pointLightCalculateLight(fromLight, &reciever->transform.position, currLight);
            gSPLight(renderState->dl++, currLight, 1);

            ++currentLight;
        }

        gSPDisplayList(renderState->dl++, reciever->geometry);
        gSPPopMatrix(renderState->dl++, G_MTX_MODELVIEW);
    }

    // calculate position of top and bottom of shadow
    Mtx* shadowMatrices = renderStateRequestMatrices(renderState, 2);
    transformToMatrixL(&shadowRenderer->casterTransform, &shadowMatrices[TOP_MATRIX_INDEX]);

    struct Vector3 lightOffset;
    vector3Sub(&shadowRenderer->casterTransform.position, &fromLight->position, &lightOffset);

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
    transformToMatrixL(&shadowEnd, &shadowMatrices[BOTTOM_MATRIX_INDEX]);

    // render back of shadows
    gDPPipeSync(renderState->dl++);
    // check if the shadow is inside out and need culling to flipped
    if (lightVerticalOffset < 0.0f) {
        gSPGeometryMode(renderState->dl++, G_CULL_BACK, G_CULL_FRONT);
    }
    gDPSetRenderMode(renderState->dl++, G_RM_ZB_XLU_SURF | Z_UPD, G_RM_ZB_XLU_SURF2 | Z_UPD);
    gSPSegment(renderState->dl++, MATRIX_TRANSFORM_SEGMENT, shadowMatrices);
    gSPDisplayList(renderState->dl++, shadow_mat);
    gSPDisplayList(renderState->dl++, shadowRenderer->shape);


    gDPPipeSync(renderState->dl++);
    gDPSetRenderMode(renderState->dl++, G_RM_ZB_OPA_DECAL, G_RM_ZB_OPA_DECAL2);

    if (lightVerticalOffset < 0.0f) {
        gSPGeometryMode(renderState->dl++, G_CULL_FRONT, G_CULL_BACK);
    }
    // second pass for shadowed objects
    for (unsigned i = 0; i < recieverCount; ++i) {
        struct ShadowReceiver* reciever = &recievers[i];

        gSPMatrix(renderState->dl++, &recieverMatrices[i], G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPDisplayList(renderState->dl++, reciever->shadowMaterial);

        if (reciever->flags & ShadowReceiverFlagsUseLight) {
            gSPLight(renderState->dl++, &gLightBlack, 1);
        }

        gSPDisplayList(renderState->dl++, reciever->geometry);
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
    gSPDisplayList(renderState->dl++, shadowRenderer->shape);

    gDPPipeSync(renderState->dl++);
    gDPSetRenderMode(renderState->dl++, G_RM_ZB_OPA_DECAL, G_RM_ZB_OPA_DECAL2);
    if (lightVerticalOffset > 0.0f) {
        gSPGeometryMode(renderState->dl++, G_CULL_FRONT, G_CULL_BACK);
    }

    // third pass for shadowed objects
    currentLight = 0;
    for (unsigned i = 0; i < recieverCount; ++i) {
        struct ShadowReceiver* reciever = &recievers[i];

        gSPMatrix(renderState->dl++, &recieverMatrices[i], G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
        gSPDisplayList(renderState->dl++, reciever->litMaterial);

        if (reciever->flags & ShadowReceiverFlagsUseLight) {
            gSPLight(renderState->dl++, &lights[currentLight], 1);
            ++currentLight;
        }

        gSPDisplayList(renderState->dl++, reciever->geometry);
        gSPPopMatrix(renderState->dl++, G_MTX_MODELVIEW);
    }
}