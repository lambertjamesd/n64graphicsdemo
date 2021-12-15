#ifndef __SHADOW_MAP_H__
#define __SHADOW_MAP_H__

#include <ultra64.h>
#include "graphics/renderstate.h"
#include "math/transform.h"
#include "shadow_map.h"
#include "point_light.h"

#define SHADOW_MAP_WIDTH    32
#define SHADOW_MAP_HEIGHT   32

void shadowMapRender(struct RenderState* renderState, struct PointLight* from, struct Transform* subjectTransform, Gfx* subject, float subjectRadius);
void shadowMapRenderDebug(struct RenderState* renderState);

#endif