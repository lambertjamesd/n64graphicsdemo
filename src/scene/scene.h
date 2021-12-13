#ifndef __SCENE_H__
#define __SCENE_H__

#include "camera.h"
#include "graphics/renderstate.h"
#include "shadow_renderer.h"
#include "point_light.h"

struct Scene {
    struct Camera camera;
    struct ShadowRenderer shadowRenderer;
    struct PointLight lightSource;
};

void sceneInit(struct Scene* scene);
void sceneRender(struct Scene* scene, struct RenderState* renderState);
void sceneUpdate(struct Scene* scene);

#endif