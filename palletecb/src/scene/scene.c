#include "scene.h"

#include "../defs.h"

#include "../graphics/copycb.h"
#include "../models/models.h"
#include "../materials/basic_lit.h"
#include "../controls/controller.h"
#include "../util/time.h"

#define ROTATE_PER_SECOND       (M_PI * 0.25f)
#define MOVE_PER_SECOND         (3.0f * SCENE_SCALE)
#define MIN_DISTANCE            (SCENE_SCALE * 2.0f)
#define MAX_DISTANCE            (SCENE_SCALE * 20.0f)

Lights1 gLights = gdSPDefLights1(0x10, 0, 0, 255, 255, 255, 0, 127, 0);

struct Vector3 gSpherePos = {0.0f, SCENE_SCALE, 0.0f};
struct Vector3 gCylinderPos = {2.0f * SCENE_SCALE, SCENE_SCALE, 0.0f};
struct Vector3 gSuzunnePos = {2.0f * SCENE_SCALE, SCENE_SCALE, 2.0f * SCENE_SCALE};

struct Vector3 gCameraFocus = {0.0f, SCENE_SCALE, 0.0f};
struct Vector3 gCameraStart = {0.0f, SCENE_SCALE * 3.0f, SCENE_SCALE * 15.0f};
float gCameraDistance = 0.0f;

void sceneInit(struct Scene* scene) {
    cameraInit(&scene->camera, 90.0f, 0.5f * SCENE_SCALE, 50.0f * SCENE_SCALE);

    gCameraDistance = sqrtf(vector3DistSqrd(&gCameraFocus, &gCameraStart));

    scene->camera.transform.position = gCameraStart;
    struct Vector3 offset;
    vector3Sub(&gCameraFocus, &gCameraStart, &offset);
    quatLook(&offset, &gUp, &scene->camera.transform.rotation);
}

unsigned ignoreInputFrames = 10;

void sceneUpdate(struct Scene* scene) {
    OSContPad* input = controllersGetControllerData(0);

    struct Quaternion rotate;
    quatAxisAngle(&gUp, (ROTATE_PER_SECOND * (1.0f / 80.0f)) * gTimeDelta * input->stick_x, &rotate);
    struct Quaternion finalRotation;
    quatMultiply(&rotate, &scene->camera.transform.rotation, &finalRotation);
    scene->camera.transform.rotation = finalRotation;

    quatAxisAngle(&gRight, -(ROTATE_PER_SECOND * (1.0f / 80.0f)) * gTimeDelta * input->stick_y, &rotate);
    quatMultiply(&scene->camera.transform.rotation, &rotate, &finalRotation);
    scene->camera.transform.rotation = finalRotation;

    if (!ignoreInputFrames && controllerGetButton(0, A_BUTTON)) {
        gCameraDistance -= MOVE_PER_SECOND * gTimeDelta;
    }

    if (!ignoreInputFrames && controllerGetButton(0, B_BUTTON)) {
        gCameraDistance += MOVE_PER_SECOND * gTimeDelta;
    }

    gCameraDistance = MAX(MIN_DISTANCE, gCameraDistance);
    gCameraDistance = MIN(MAX_DISTANCE, gCameraDistance);

    struct Vector3 offset;
    quatMultVector(&scene->camera.transform.rotation, &gForward, &offset);
    vector3AddScaled(&gCameraFocus, &offset, gCameraDistance, &scene->camera.transform.position);

    if (ignoreInputFrames) {
        --ignoreInputFrames;
    }
}

void sceneRender(struct Scene* scene, struct RenderState* renderState, struct GraphicsTask* task) {
    gDPPipeSync(renderState->dl++);
    gDPSetColorImage(renderState->dl++, G_IM_FMT_CI, G_IM_SIZ_8b, SCREEN_WD, indexColorBuffer);
    gDPSetCycleType(renderState->dl++, G_CYC_FILL);
    gDPSetFillColor(renderState->dl++, 0x08080808);
    gDPFillRectangle(renderState->dl++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
    gDPPipeSync(renderState->dl++);
    gDPSetCycleType(renderState->dl++, G_CYC_1CYCLE);
    gDPSetRenderMode(renderState->dl++, G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2);

    cameraSetupMatrices(&scene->camera, renderState, (float)SCREEN_WD / (float)SCREEN_HT);
    
    gSPSetLights1(renderState->dl++, gLights);
    gSPDisplayList(renderState->dl++, gBasicLitMaterial);

    struct Transform transform;
    transformInitIdentity(&transform);

    transform.position = gSpherePos;
    Mtx* sphereTransform = renderStateRequestMatrices(renderState, 1);
    transformToMatrixL(&transform, sphereTransform);
    gSPMatrix(renderState->dl++, sphereTransform, G_MTX_MODELVIEW | G_MTX_PUSH | G_MTX_MUL);
    gSPDisplayList(renderState->dl++, sphere_model_gfx);
    gSPPopMatrix(renderState->dl++, G_MTX_MODELVIEW);

    transform.position = gCylinderPos;
    Mtx* cylinderTransform = renderStateRequestMatrices(renderState, 1);
    transformToMatrixL(&transform, cylinderTransform);
    gSPMatrix(renderState->dl++, cylinderTransform, G_MTX_MODELVIEW | G_MTX_PUSH | G_MTX_MUL);
    gSPDisplayList(renderState->dl++, cylinder_model_gfx);
    gSPPopMatrix(renderState->dl++, G_MTX_MODELVIEW);

    transform.position = gSuzunnePos;
    Mtx* suzanneTransform = renderStateRequestMatrices(renderState, 1);
    transformToMatrixL(&transform, suzanneTransform);
    gSPMatrix(renderState->dl++, suzanneTransform, G_MTX_MODELVIEW | G_MTX_PUSH | G_MTX_MUL);
    gSPDisplayList(renderState->dl++, suzanne_model_gfx);
    gSPPopMatrix(renderState->dl++, G_MTX_MODELVIEW);

    transform.position = gZeroVec;
    Mtx* groundTransform = renderStateRequestMatrices(renderState, 1);
    transformToMatrixL(&transform, groundTransform);
    gSPMatrix(renderState->dl++, groundTransform, G_MTX_MODELVIEW | G_MTX_PUSH | G_MTX_MUL);
    gSPDisplayList(renderState->dl++, ground_model_gfx);
    gSPPopMatrix(renderState->dl++, G_MTX_MODELVIEW);

    gDPPipeSync(renderState->dl++);
    gDPSetCycleType(renderState->dl++, G_CYC_1CYCLE);
    gDPSetColorImage(renderState->dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, osVirtualToPhysical(task->framebuffer));
    gSPSegment(renderState->dl++, SOURCE_CB_SEGMENT, indexColorBuffer);
    gSPDisplayList(renderState->dl++, gCopyCB);
}