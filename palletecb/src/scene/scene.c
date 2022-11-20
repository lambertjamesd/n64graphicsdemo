#include "scene.h"

#include "../defs.h"

#include "../graphics/copycb.h"
#include "../models/models.h"
#include "../materials/basic_lit.h"
#include "../materials/toon_lit.h"
#include "../materials/outline_pass.h"
#include "../materials/point_light_rendered.h"
#include "../controls/controller.h"
#include "../util/time.h"

#define ROTATE_PER_SECOND       (M_PI * 0.25f)
#define MOVE_PER_SECOND         (3.0f * SCENE_SCALE)
#define MIN_DISTANCE            (SCENE_SCALE * 2.0f)
#define MAX_DISTANCE            (SCENE_SCALE * 20.0f)

Lights1 gLights = gdSPDefLights1(0x10, 0, 0, 0xE0, 0xE0, 0xE0, 90, 90, 0);

struct Vector3 gCameraFocus = {0.0f, SCENE_SCALE, 0.0f};
struct Vector3 gCameraStart = {SCENE_SCALE * -2.0f, SCENE_SCALE * 5.0f, SCENE_SCALE * 5.0f};
float gCameraDistance = 0.0f;

#define OBJECT_COUNT    3

struct Transform gObjectPos[OBJECT_COUNT] = {
    {{-1.5f * SCENE_SCALE, SCENE_SCALE, -2.5f * SCENE_SCALE}, {0.707f, 0.0f, 0.0f, 0.707f}, {1.0f, 1.0f, 1.0f}},
    {{2.5f * SCENE_SCALE, SCENE_SCALE, -2.0f * SCENE_SCALE}, {0.707f, 0.0f, 0.0f, 0.707f}, {1.0f, 1.0f, 1.0f}},
    {{0.0f * SCENE_SCALE, SCENE_SCALE * 0.75f, 1.5f * SCENE_SCALE}, {-0.923879533f, 0.0f, 0.0f, 0.382683432f}, {1.0f, 1.0f, 1.0f}},
};

Gfx* gObjectGfx[OBJECT_COUNT] = {
    sphere_model_gfx,
    cylinder_model_gfx,
    suzanne_model_gfx,
};

struct Vector3 gLightOrbitCenter = {0.0f, SCENE_SCALE * 5.0f, 0.0f};

#define LIGHT_ORBIT_RADIUS  (5.0f * SCENE_SCALE)
#define LIGHT_ORBIT_PERIOD  3.0f

void materialSetBasicLit(struct RenderState* renderState, int objectIndex) {
    gSPDisplayList(renderState->dl++, gBasicLitMaterial);
}

void materialSetToon(struct RenderState* renderState, int objectIndex) {
    toonLitUse(renderState, 5 + 4 * objectIndex, 2);
}

void materialYesWeCan(struct RenderState* renderState, int objectIndex) {
    toonLitUse(renderState, 64, 128);
}

void materialSetOutline(struct RenderState* renderState, int objectIndex) {
    gSPDisplayList(renderState->dl++, gOutlinePass);
}

#define GROUND_LERP  TEXEL0, 0, ENVIRONMENT, PRIMITIVE, 0, 0, 0, ENVIRONMENT

void materialToonGround(struct RenderState* renderState, int objectIndex) {
    toonLitUse(renderState, 5 + 4 * OBJECT_COUNT, 2);
    gDPSetCombineMode(renderState->dl++, GROUND_LERP, GROUND_LERP);
}

void materialYesWeCanGround(struct RenderState* renderState, int objectIndex) {
    toonLitUse(renderState, 64, 128);
    gDPSetCombineMode(renderState->dl++, GROUND_LERP, GROUND_LERP);
}

struct RenderModeData gRenderModeData[] = {
    {RenderModeFlagsAttenuate, gFirePallete, 0x08080808, materialSetBasicLit, 0, 0},
    {RenderModeFlagsAttenuate, gIcePallete, 0x08080808, materialSetBasicLit, 0, 0},
    {RenderModeFlagsAttenuate, gHeat, 0x08080808, materialSetBasicLit, 0, 0},
    {RenderModeFlagsAttenuate, gRainbow, 0x08080808, materialSetBasicLit, 0, 0},
    {RenderModeFlagsAttenuate, gGrayscale, 0x00000000, materialSetBasicLit, 0, 0},
    {0, gToonPallete, 0x00000000, materialSetToon, materialSetOutline, materialToonGround},
    {0, gYesWeCan, 0x00000000, materialYesWeCan, materialSetOutline, materialYesWeCanGround},
};

void sceneInit(struct Scene* scene) {
    cameraInit(&scene->camera, 90.0f, 0.5f * SCENE_SCALE, 50.0f * SCENE_SCALE);

    gCameraDistance = sqrtf(vector3DistSqrd(&gCameraFocus, &gCameraStart));

    scene->camera.transform.position = gCameraStart;
    struct Vector3 offset;
    vector3Sub(&gCameraFocus, &gCameraStart, &offset);
    quatLook(&offset, &gUp, &scene->camera.transform.rotation);

    scene->renderMode = RenderModeToon;

    pointLightInit(&scene->pointLight, &gLightOrbitCenter, &gColorWhite, 15.0f);
    pointLightableMeshInit(&scene->ground, ground_model_vtx, ground_model_gfx, &gColorWhite);
    struct Transform transform;
    transformInitIdentity(&transform);
    pointLightableCalc(&scene->ground, &transform, &scene->pointLight);
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

    if (!ignoreInputFrames && controllerGetButtonDown(0, R_TRIG)) {
        scene->renderMode = (scene->renderMode + 1) % RenderModeCount;
    }

    if (!ignoreInputFrames && controllerGetButtonDown(0, L_TRIG | Z_TRIG)) {
        if (scene->renderMode == 0) {
            scene->renderMode = RenderModeCount - 1;
        } else {
            --scene->renderMode;
        }
    }

    gCameraDistance = MAX(MIN_DISTANCE, gCameraDistance);
    gCameraDistance = MIN(MAX_DISTANCE, gCameraDistance);

    struct Vector3 offset;
    quatMultVector(&scene->camera.transform.rotation, &gForward, &offset);
    vector3AddScaled(&gCameraFocus, &offset, gCameraDistance, &scene->camera.transform.position);

    float angle = gTimePassed * 2.0f * M_PI / LIGHT_ORBIT_PERIOD;

    scene->pointLight.position.x = cosf(angle) * LIGHT_ORBIT_RADIUS + gLightOrbitCenter.x;
    scene->pointLight.position.y = cosf(angle * 3.0f) * SCENE_SCALE + gLightOrbitCenter.y;
    scene->pointLight.position.z = sinf(angle) * LIGHT_ORBIT_RADIUS + gLightOrbitCenter.z;

    struct Transform transform;
    transformInitIdentity(&transform);
    pointLightableCalc(&scene->ground, &transform, &scene->pointLight);

    if (ignoreInputFrames) {
        --ignoreInputFrames;
    }
}

void sceneRenderObject(struct Scene* scene, struct RenderState* renderState, struct RenderModeData* renderMode, Gfx* model, struct Transform* transform, int objectIndex) {
    Mtx* mtxTransform = renderStateRequestMatrices(renderState, 1);

    Light* light = renderStateRequestLights(renderState, 1);

    if (renderMode->flags & RenderModeFlagsAttenuate) {
        pointLightCalculateLight(&scene->pointLight, &transform->position, light);
    } else {
        pointLightCalculateLightDirOnly(&scene->pointLight, &transform->position, light);
    }

    gSPLight(renderState->dl++, light, 1);

    transformToMatrixL(transform, mtxTransform);
    renderMode->setObjectMaterial(renderState, objectIndex);
    gSPMatrix(renderState->dl++, mtxTransform, G_MTX_MODELVIEW | G_MTX_PUSH | G_MTX_MUL);
    gSPDisplayList(renderState->dl++, model);

    if (renderMode->secondObjectPass) {
        renderMode->secondObjectPass(renderState, objectIndex);
        gSPDisplayList(renderState->dl++, model);
    }

    gSPPopMatrix(renderState->dl++, G_MTX_MODELVIEW);
}

void sceneRender(struct Scene* scene, struct RenderState* renderState, struct GraphicsTask* task) {
    struct RenderModeData* renderMode = &gRenderModeData[scene->renderMode];

    gDPPipeSync(renderState->dl++);
    gDPSetColorImage(renderState->dl++, G_IM_FMT_CI, G_IM_SIZ_8b, SCREEN_WD, indexColorBuffer);
    gDPSetCycleType(renderState->dl++, G_CYC_FILL);
    gDPSetFillColor(renderState->dl++, gRenderModeData[scene->renderMode].clearColor);
    gDPFillRectangle(renderState->dl++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
    gDPPipeSync(renderState->dl++);
    gDPSetCycleType(renderState->dl++, G_CYC_1CYCLE);
    gDPSetRenderMode(renderState->dl++, G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2);

    cameraSetupMatrices(&scene->camera, renderState, (float)SCREEN_WD / (float)SCREEN_HT);
    
    gSPSetLights1(renderState->dl++, gLights);

    for (unsigned i = 0; i < OBJECT_COUNT; ++i) {
        sceneRenderObject(
            scene, 
            renderState, 
            renderMode, 
            gObjectGfx[i], 
            &gObjectPos[i], 
            i
        );
    }

    gSPGeometryMode(renderState->dl++, G_CULL_FRONT, G_CULL_BACK);
    gDPSetRenderMode(renderState->dl++, G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2);

    struct Transform transform;
    transformInitIdentity(&transform);

    gSPDisplayList(renderState->dl++, point_light_mat);
    pointLightableSetMaterial(&scene->ground, renderState, &gColorBlack);
    if (renderMode->groundMaterial) {
        renderMode->groundMaterial(renderState, 0);
    }
    gSPDisplayList(renderState->dl++, scene->ground.drawCommand);

    Mtx* lightMtx = renderStateRequestMatrices(renderState, 1);
    transform.position = scene->pointLight.position;
    vector3Scale(&gOneVec, &transform.scale, 0.25f);
    transformToMatrixL(&transform, lightMtx);

    gDPSetEnvColor(renderState->dl++, 255, 255, 255, 255);
    gDPSetCombineLERP(renderState->dl++, 0, 0, 0, ENVIRONMENT, 0, 0, 0, ENVIRONMENT, 0, 0, 0, ENVIRONMENT, 0, 0, 0, ENVIRONMENT);
    gSPMatrix(renderState->dl++, lightMtx, G_MTX_MODELVIEW | G_MTX_PUSH | G_MTX_MUL);
    gSPDisplayList(renderState->dl++, sphere_model_gfx);
    gSPPopMatrix(renderState->dl++, G_MTX_MODELVIEW);

    gDPPipeSync(renderState->dl++);
    gDPSetCycleType(renderState->dl++, G_CYC_1CYCLE);
    gDPSetColorImage(renderState->dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, osVirtualToPhysical(task->framebuffer));
    gSPSegment(renderState->dl++, SOURCE_CB_SEGMENT, indexColorBuffer);

    gDPLoadTLUT_pal256(renderState->dl++, gRenderModeData[scene->renderMode].pallete);

    gSPDisplayList(renderState->dl++, gCopyCB);
}