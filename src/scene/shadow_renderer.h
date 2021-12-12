#ifndef __SHADOW_RENDERER_H__
#define __SHADOW_RENDERER_H__

#include <ultra64.h>

#include "graphics/renderstate.h"
#include "math/transform.h"

struct ShadowReceiver {
    Gfx* litMaterial;
    Gfx* shadowMaterial;
    Gfx* geometry;
};

struct ShadowRenderer {
    Gfx* gfx;
    struct Transform casterTransform;
    struct Vector3 lightPosition;
    float shadowLength;
    unsigned char casterBoneIndex;
    unsigned char bottomShadowIndex;
};

void shadowRendererInit(struct ShadowRenderer* shadowRenderer, Gfx* gfx, unsigned casterBoneIndex, unsigned bottomShadowIndex, float shadowLength);
void shadowRendererRender(
    struct ShadowRenderer* shadowRenderer, 
    struct RenderState* renderState, 
    struct ShadowReceiver* recievers, 
    Mtx* recieverMatrices,
    unsigned recieverCount
);

#endif