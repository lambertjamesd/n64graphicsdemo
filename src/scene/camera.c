
#include "camera.h"
#include "math/transform.h"

void cameraInit(struct Camera* camera, float fov, float near, float far) {
    transformInitIdentity(&camera->transform);
    camera->fov = fov;
    camera->nearPlane = near;
    camera->farPlane = far;
}

void cameraBuildViewMatrix(struct Camera* camera, Mtx* matrix) {
    struct Transform cameraTransCopy = camera->transform;
    struct Transform inverse;
    transformInvert(&cameraTransCopy, &inverse);
    transformToMatrixL(&inverse, matrix);
}

void cameraBuildProjectionMatrix(struct Camera* camera, Mtx* matrix, u16* perspectiveNormalize, float aspectRatio) {
    float planeScalar = 1.0f;

    if (camera->transform.position.y > camera->farPlane * 0.5f) {
        planeScalar = 2.0f * camera->transform.position.y / camera->farPlane;
    }

    guPerspective(matrix, perspectiveNormalize, camera->fov, aspectRatio, camera->nearPlane * planeScalar, camera->farPlane * planeScalar, 1.0f);
}

void cameraSetupMatrices(struct Camera* camera, struct RenderState* renderState, float aspectRatio) {
    Mtx* viewProjMatrix = renderStateRequestMatrices(renderState, 2);
    
    if (!viewProjMatrix) {
        return;
    }

    cameraBuildViewMatrix(camera, &viewProjMatrix[0]);
    gSPMatrix(renderState->dl++, osVirtualToPhysical(&viewProjMatrix[0]), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);

    u16 perspectiveNormalize;
    cameraBuildProjectionMatrix(camera, &viewProjMatrix[1], &perspectiveNormalize, aspectRatio);
    gSPMatrix(renderState->dl++, osVirtualToPhysical(&viewProjMatrix[1]), G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPPerspNormalize(renderState->dl++, perspectiveNormalize);
}