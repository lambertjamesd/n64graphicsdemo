#ifndef __SCENE__H
#define __SCENE__H

#include "../graphics/renderstate.h"
#include "../graphics/graphics.h"

#include "point_light.h"
#include "camera.h"

enum RenderMode {
    RenderModeRainbow,
    RenderModeCount,
};

typedef void (*SetObjectMaterial)(struct RenderState* renderState, int objectIndex);

enum RenderModeFlags {
    RenderModeFlagsAttenuate = (1 << 0),
};

struct RenderModeData {
    enum RenderModeFlags flags;
    u64* pallete;
    u32 clearColor;
    SetObjectMaterial setObjectMaterial;
    SetObjectMaterial secondObjectPass;
    SetObjectMaterial groundMaterial;
};

struct Scene {
    struct Camera camera;
    enum RenderMode renderMode;
    struct PointLight pointLight;
};

void sceneInit(struct Scene* scene);
void sceneUpdate(struct Scene* scene);
void sceneRender(struct Scene* scene, struct RenderState* renderState, struct GraphicsTask* task);

#endif