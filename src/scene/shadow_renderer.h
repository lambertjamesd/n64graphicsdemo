#ifndef __SHADOW_RENDERER_H__
#define __SHADOW_RENDERER_H__

#include <ultra64.h>

#include "graphics/renderstate.h"
#include "math/transform.h"
#include "graphics/color.h"
#include "point_light.h"

enum ShadowReceiverFlags {
    ShadowReceiverFlagsUseLight = (1 << 0),
};

struct ShadowReceiver {
    Gfx* litMaterial;
    Gfx* shadowMaterial;
    Gfx* geometry;
    unsigned short flags;
    struct Transform transform;
};

struct ShadowRenderer {
    Gfx* shape;
    struct Transform casterTransform;
    float shadowLength;
    unsigned char casterBoneIndex;
    unsigned char bottomShadowIndex;

};

void shadowRendererInit(struct ShadowRenderer* shadowRenderer, Gfx* shape, unsigned casterBoneIndex, unsigned bottomShadowIndex, float shadowLength);
void shadowRendererRender(
    struct ShadowRenderer* shadowRenderer, 
    struct RenderState* renderState, 
    struct PointLight* fromLight,
    struct ShadowReceiver* recievers, 
    unsigned recieverCount
);

#endif