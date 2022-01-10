#ifndef __SCENE__H
#define __SCENE__H

#include "../graphics/renderstate.h"

struct Scene {

};

void sceneInit(struct Scene* scene);
void sceneUpdate(struct Scene* scene);
void sceneRender(struct Scene* scene, struct RenderState* renderState);

#endif