#ifndef __SCENE__H
#define __SCENE__H

#include "../graphics/renderstate.h"
#include "../graphics/graphics.h"

struct Scene {

};

void sceneInit(struct Scene* scene);
void sceneUpdate(struct Scene* scene);
void sceneRender(struct Scene* scene, struct RenderState* renderState, struct GraphicsTask* task);

#endif