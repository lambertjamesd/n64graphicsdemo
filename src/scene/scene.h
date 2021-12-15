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
    struct PointLightableMesh groundMesh;
    struct PointLightableMesh casterMesh;
};

struct GraphicsTask;

void sceneInit(struct Scene* scene);
void sceneRender(struct Scene* scene, struct RenderState* renderState, struct GraphicsTask* task);
void sceneUpdate(struct Scene* scene);

#endif