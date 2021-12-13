
#include "scene.h"

#include "defs.h"
#include "graphics/graphics.h"
#include "models/models.h"
#include "materials/shadow_caster.h"
#include "materials/ground.h"
#include "materials/subject.h"
#include "util/time.h"
#include "sk64/skelatool_defs.h"

struct Vector3 gCameraFocus = {0.0f, SCENE_SCALE, 0.0f};
struct Vector3 gCameraStart = {0.0f, SCENE_SCALE * 3.0f, SCENE_SCALE * 7.0f};
struct Vector3 gShadowCasterPos = {0.0f, SCENE_SCALE * 2.0f, 0.0f};
struct Vector3 gLightPosition = {0.0f, SCENE_SCALE * 6.0f, 0.0f};
float gCameraDistance = 0.0f;

struct ShadowReceiver gRecieviers[] = {
    {subject_mat, subject_mat, subject_model_gfx, ShadowReceiverFlagsUseLight},
};

struct Vector2 gSquareShadow[] = {
    {SCENE_SCALE, SCENE_SCALE},
    {SCENE_SCALE, -SCENE_SCALE},
    {-SCENE_SCALE, -SCENE_SCALE},
    {-SCENE_SCALE, SCENE_SCALE},
};

#define ROTATE_PER_SECOND       (M_PI * 0.25f)

Lights2 static_light = gdSPDefLights2(
    32, 32, 32, 
    0, 0, 0, 0, 0x7f, 0,
    2, 16, 32, 0, 0x81, 0
);

void sceneInit(struct Scene* scene) {
    cameraInit(&scene->camera, 45.0f, 50.0f, 5000.0f);

    scene->camera.transform.position = gCameraStart;
    struct Vector3 offset;
    vector3Sub(&gCameraFocus, &gCameraStart, &offset);
    quatLook(&offset, &gUp, &scene->camera.transform.rotation);
    gCameraDistance = sqrtf(vector3DistSqrd(&gCameraFocus, &gCameraStart));

    shadowRendererInit(&scene->shadowRenderer, gSquareShadow, sizeof(gSquareShadow)/sizeof(*gSquareShadow), SCENE_SCALE * 4.0f);
    scene->shadowRenderer.casterTransform.position = gShadowCasterPos;

    pointLightInit(&scene->lightSource, &gLightPosition, &gColorWhite, 1.0f);
}

void sceneRender(struct Scene* scene, struct RenderState* renderState) {

    cameraSetupMatrices(&scene->camera, renderState, (float)SCREEN_WD / (float)SCREEN_HT);
    gDPSetRenderMode(renderState->dl++, G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2);

    gSPDisplayList(renderState->dl++, ground_mat);
    gSPDisplayList(renderState->dl++, ground_model_gfx);

    struct Vector3 angles;
    angles.x = gTimePassed * (M_PI * 50.0f / 180.0f);
    angles.y = gTimePassed * (M_PI * 30.0f / 180.0f);
    angles.z = 0.0f;
    vector3Scale(&gOneVec, &gRecieviers[0].transform.scale, 0.5f);
    gRecieviers[0].transform.position.x = gCameraFocus.x + SCENE_SCALE * 1.5f * cosf(gTimePassed);
    gRecieviers[0].transform.position.y = gCameraFocus.y;
    gRecieviers[0].transform.position.z = gCameraFocus.z + SCENE_SCALE * 1.5f * cosf(gTimePassed * 0.33f);
    quatEulerAngles(&angles, &gRecieviers[0].transform.rotation);

    scene->lightSource.position.x = gLightPosition.x + SCENE_SCALE * 1.5f * cosf(gTimePassed);
    scene->lightSource.position.y = gLightPosition.y + SCENE_SCALE * 0.25f * cosf(gTimePassed * 0.5f);
    scene->lightSource.position.z = gLightPosition.z + SCENE_SCALE * 1.5f * cosf(gTimePassed * 2.0f);

    Mtx* casterMatrix = renderStateRequestMatrices(renderState, 1);

    guTranslate(casterMatrix, gShadowCasterPos.x, gShadowCasterPos.y, gShadowCasterPos.z);

    gSPSetLights2(renderState->dl++, static_light);

    Light* pointLightSource = renderStateRequestLights(renderState, 1);
    pointLightCalculateLight(&scene->lightSource, &gShadowCasterPos, pointLightSource);
    gSPLight(renderState->dl++, pointLightSource, 1);
    gSPMatrix(renderState->dl++, casterMatrix, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPDisplayList(renderState->dl++, shadow_caster_mat);
    gSPDisplayList(renderState->dl++, shadow_caster_model_gfx);
    gSPPopMatrix(renderState->dl++, G_MTX_MODELVIEW);

    shadowRendererRender(
        &scene->shadowRenderer,
        renderState,
        &scene->lightSource,
        gRecieviers,
        sizeof(gRecieviers) / sizeof(*gRecieviers)
    );
}

void sceneUpdate(struct Scene* scene) {
    struct Quaternion rotate;
    quatAxisAngle(&gUp, ROTATE_PER_SECOND * gTimeDelta, &rotate);
    struct Quaternion finalRotation;
    quatMultiply(&rotate, &scene->camera.transform.rotation, &finalRotation);
    scene->camera.transform.rotation = finalRotation;

    struct Vector3 offset;
    quatMultVector(&scene->camera.transform.rotation, &gForward, &offset);
    vector3AddScaled(&gCameraFocus, &offset, gCameraDistance, &scene->camera.transform.position);
}