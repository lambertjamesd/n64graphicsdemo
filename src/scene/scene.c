
#include "scene.h"

#include "defs.h"
#include "graphics/graphics.h"
#include "models/models.h"
#include "materials/shadow_caster.h"
#include "materials/shadow.h"
#include "materials/ground.h"
#include "util/time.h"
#include "sk64/skelatool_defs.h"

struct Vector3 gCameraFocus = {0.0f, SCENE_SCALE, 0.0f};
struct Vector3 gCameraStart = {0.0f, SCENE_SCALE * 3.0f, SCENE_SCALE * 7.0f};
struct Vector3 gShadowCasterPos = {0.0f, SCENE_SCALE * 2.0f, 0.0f};
float gCameraDistance = 0.0f;

struct ShadowReceiver gRecieviers[] = {
    {ground_mat, ground_in_shadow_mat, ground_model_gfx},
    {ground_mat, ground_in_shadow_mat, subject_model_gfx},
};

#define ROTATE_PER_SECOND       (M_PI * 0.25f)

void sceneInit(struct Scene* scene) {
    cameraInit(&scene->camera, 45.0f, 50.0f, 5000.0f);

    scene->camera.transform.position = gCameraStart;
    struct Vector3 offset;
    vector3Sub(&gCameraFocus, &gCameraStart, &offset);
    quatLook(&offset, &gUp, &scene->camera.transform.rotation);
    gCameraDistance = sqrtf(vector3DistSqrd(&gCameraFocus, &gCameraStart));

    shadowRendererInit(&scene->shadowRenderer, cube_shadow_model_gfx, CUBE_SHADOW_SHADOWTOP_BONE, CUBE_SHADOW_SHADOWBOTTOM_BONE, SCENE_SCALE * 4.0f);
    scene->shadowRenderer.casterTransform.position = gShadowCasterPos;
    vector3Scale(&gUp, &scene->shadowRenderer.lightPosition, SCENE_SCALE * 6.0f);
}

void sceneRender(struct Scene* scene, struct RenderState* renderState) {

    cameraSetupMatrices(&scene->camera, renderState, (float)SCREEN_WD / (float)SCREEN_HT);
    gDPSetRenderMode(renderState->dl++, G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2);

    Mtx* shadowMatrices = renderStateRequestMatrices(renderState, CUBE_SHADOW_DEFAULT_BONES_COUNT);

    guTranslate(&shadowMatrices[CUBE_SHADOW_SHADOWTOP_BONE], gShadowCasterPos.x, gShadowCasterPos.y, gShadowCasterPos.z);
    guTranslate(&shadowMatrices[CUBE_SHADOW_SHADOWBOTTOM_BONE], 0.0f, -gShadowCasterPos.y, 0.0f);

    Mtx* subjectMatrices = renderStateRequestMatrices(renderState, 2);
    guMtxIdent(&subjectMatrices[0]);
    
    guPosition(
        &subjectMatrices[1], 
        gTimePassed * 50.0f, gTimePassed * 30.0f, 0.0f, 
        0.5f, 
        gCameraFocus.x + SCENE_SCALE * 1.5f * cosf(gTimePassed), 
        gCameraFocus.y, 
        gCameraFocus.z + SCENE_SCALE * 1.5f * cosf(gTimePassed * 0.33f)
    );

    Mtx* casterMatrix = renderStateRequestMatrices(renderState, 1);

    guTranslate(casterMatrix, gShadowCasterPos.x, gShadowCasterPos.y, gShadowCasterPos.z);

    gSPMatrix(renderState->dl++, casterMatrix, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPDisplayList(renderState->dl++, shadow_caster_mat);
    gSPDisplayList(renderState->dl++, shadow_caster_model_gfx);
    gSPPopMatrix(renderState->dl++, G_MTX_MODELVIEW);

    shadowRendererRender(
        &scene->shadowRenderer,
        renderState,
        gRecieviers,
        subjectMatrices,
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