#ifndef _CAMERA_H
#define _CAMERA_H

#include <ultra64.h>

#include "math/quaternion.h"
#include "math/vector3.h"
#include "math/transform.h"
#include "graphics/renderstate.h"

struct Camera {
    struct Transform transform;
    float nearPlane;
    float farPlane;
    float fov;
};

void cameraInit(struct Camera* camera, float fov, float near, float far);
void cameraBuildViewMatrix(struct Camera* camera, Mtx* matrix);
void cameraBuildProjectionMatrix(struct Camera* camera, Mtx* matrix, u16* perspectiveNorm, float aspectRatio);
void cameraSetupMatrices(struct Camera* camera, struct RenderState* renderState, float aspectRatio);

#endif