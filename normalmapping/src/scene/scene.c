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

Lights1 gLights = gdSPDefLights1(0, 0, 0, 0, 0, 0, 90, 90, 0);

struct Vector3 gCameraFocus = {0.0f, 0.0f, 0.0f};
struct Vector3 gCameraStart = {SCENE_SCALE * -2.0f, SCENE_SCALE * 5.0f, SCENE_SCALE * 5.0f};
float gCameraDistance = 0.0f;

struct Vector3 gLightOrbitCenter = {0.0f, SCENE_SCALE * 5.0f, 0.0f};

#define LIGHT_ORBIT_RADIUS  (5.0f * SCENE_SCALE)
#define LIGHT_ORBIT_PERIOD  3.0f

void materialSetBasicLit(struct RenderState* renderState, int objectIndex) {
    gSPDisplayList(renderState->dl++, gBasicLitMaterial);
}

#define GROUND_LERP  TEXEL0, 0, ENVIRONMENT, PRIMITIVE, 0, 0, 0, ENVIRONMENT

struct RenderModeData gRenderModeData[] = {
    {RenderModeFlagsAttenuate, gPalleteMapping, 0x08080808, materialSetBasicLit, 0, 0},
};

void sceneInit(struct Scene* scene) {
    cameraInit(&scene->camera, 90.0f, 0.5f * SCENE_SCALE, 50.0f * SCENE_SCALE);

    gCameraDistance = sqrtf(vector3DistSqrd(&gCameraFocus, &gCameraStart));

    scene->camera.transform.position = gCameraStart;
    struct Vector3 offset;
    vector3Sub(&gCameraFocus, &gCameraStart, &offset);
    quatLook(&offset, &gUp, &scene->camera.transform.rotation);

    scene->renderMode = RenderModeRainbow;

    struct Coloru8 color;
    color.r = 255;
    color.g = 255;
    color.b = 255;
    pointLightInit(&scene->pointLight, &gLightOrbitCenter, &color, 15.0f);

    quatIdent(&scene->lightRotation);
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

    if (!ignoreInputFrames && controllerGetButton(0, D_CBUTTONS)) {
        struct Quaternion rotateAmount;
        quatAxisAngle(&gRight, ROTATE_PER_SECOND * gTimeDelta, &rotateAmount);
        struct Quaternion tmp;
        quatMultiply(&scene->lightRotation, &rotateAmount, &tmp);
        scene->lightRotation = tmp; 
    }

    if (!ignoreInputFrames && controllerGetButton(0, U_CBUTTONS)) {
        struct Quaternion rotateAmount;
        quatAxisAngle(&gRight, -ROTATE_PER_SECOND * gTimeDelta, &rotateAmount);
        struct Quaternion tmp;
        quatMultiply(&scene->lightRotation, &rotateAmount, &tmp);
        scene->lightRotation = tmp; 
    }

    if (!ignoreInputFrames && controllerGetButton(0, R_CBUTTONS)) {
        struct Quaternion rotateAmount;
        quatAxisAngle(&gUp, ROTATE_PER_SECOND * gTimeDelta, &rotateAmount);
        struct Quaternion tmp;
        quatMultiply(&rotateAmount, &scene->lightRotation, &tmp);
        scene->lightRotation = tmp; 
    }

    if (!ignoreInputFrames && controllerGetButton(0, L_CBUTTONS)) {
        struct Quaternion rotateAmount;
        quatAxisAngle(&gUp, -ROTATE_PER_SECOND * gTimeDelta, &rotateAmount);
        struct Quaternion tmp;
        quatMultiply(&rotateAmount, &scene->lightRotation, &tmp);
        scene->lightRotation = tmp; 
    }

    quatMultVector(&scene->lightRotation, &gLightOrbitCenter, &scene->pointLight.position);

    gCameraDistance = MAX(MIN_DISTANCE, gCameraDistance);
    gCameraDistance = MIN(MAX_DISTANCE, gCameraDistance);

    struct Vector3 offset;
    quatMultVector(&scene->camera.transform.rotation, &gForward, &offset);
    vector3AddScaled(&gCameraFocus, &offset, gCameraDistance, &scene->camera.transform.position);

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

#define	RB_ZB_ADD_DECAL(clk)					            \
	Z_CMP | IM_RD | CVG_DST_FULL | FORCE_BL | ZMODE_DEC |	\
	GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1)

void sceneRender(struct Scene* scene, struct RenderState* renderState, struct GraphicsTask* task) {
    gDPPipeSync(renderState->dl++);
    gDPSetCycleType(renderState->dl++, G_CYC_FILL);

    // clear color buffer
    gDPSetColorImage(renderState->dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, colorBuffer);
    gDPSetFillColor(renderState->dl++, (GPACK_RGBA5551(32, 200, 128, 1) << 16) | GPACK_RGBA5551(32, 200, 128, 1));
    gDPFillRectangle(renderState->dl++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
    gDPPipeSync(renderState->dl++);

    // clear lightness buffer
    gDPSetColorImage(renderState->dl++, G_IM_FMT_CI, G_IM_SIZ_8b, SCREEN_WD, lightnessDrawBuffer);
    gDPSetFillColor(renderState->dl++, 0xFFFFFFFF);
    gDPFillRectangle(renderState->dl++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
    gDPPipeSync(renderState->dl++);

    // draw to color buffer
    gDPSetColorImage(renderState->dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, colorBuffer);
    cameraSetupMatrices(&scene->camera, renderState, (float)SCREEN_WD / (float)SCREEN_HT);
    gSPSetLights1(renderState->dl++, gLights);
    gDPSetCycleType(renderState->dl++, G_CYC_1CYCLE);
    gSPGeometryMode(renderState->dl++, G_CULL_FRONT, G_CULL_BACK);
    gDPSetRenderMode(renderState->dl++, G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2);
    gDPSetTexturePersp(renderState->dl++, G_TP_PERSP);

    struct Transform cubeTransform;
    transformInitIdentity(&cubeTransform);
    Mtx* cubeMatrix = renderStateRequestMatrices(renderState, 1);
    
    transformToMatrixL(&cubeTransform, cubeMatrix);
    gSPMatrix(renderState->dl++, cubeMatrix, G_MTX_MODELVIEW | G_MTX_PUSH | G_MTX_MUL);
    gSPDisplayList(renderState->dl++, brick_color_mat);
    gSPDisplayList(renderState->dl++, brick_color_gfx);
    gDPPipeSync(renderState->dl++);
    gSPPopMatrix(renderState->dl++, G_MTX_MODELVIEW);

    // gSPGeometryMode(renderState->dl++, G_CULL_FRONT, G_CULL_BACK);
    // gDPSetRenderMode(renderState->dl++, G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2);

    struct Transform lightTransform;
    transformInitIdentity(&lightTransform);

    Mtx* lightMtx = renderStateRequestMatrices(renderState, 1);
    lightTransform.position = scene->pointLight.position;
    vector3Scale(&gOneVec, &lightTransform.scale, 0.25f);
    transformToMatrixL(&lightTransform, lightMtx);

    gDPSetEnvColor(renderState->dl++, 255, 255, 255, 255);
    gDPSetCombineLERP(renderState->dl++, 0, 0, 0, ENVIRONMENT, 0, 0, 0, ENVIRONMENT, 0, 0, 0, ENVIRONMENT, 0, 0, 0, ENVIRONMENT);
    gSPMatrix(renderState->dl++, lightMtx, G_MTX_MODELVIEW | G_MTX_PUSH | G_MTX_MUL);
    gSPDisplayList(renderState->dl++, sphere_model_gfx);
    gSPPopMatrix(renderState->dl++, G_MTX_MODELVIEW);

    // draw to lightness buffer
    gDPPipeSync(renderState->dl++);
    gDPSetColorImage(renderState->dl++, G_IM_FMT_CI, G_IM_SIZ_8b, SCREEN_WD, lightnessDrawBuffer);
    gDPSetRenderMode(renderState->dl++, G_RM_ZB_OPA_DECAL, G_RM_ZB_OPA_DECAL2);
    gSPGeometryMode(renderState->dl++, 0, G_LIGHTING | G_SHADE);

    Light* light = renderStateRequestLights(renderState, 1);
    pointLightCalculateLightDirOnly(&scene->pointLight, &cubeTransform.position, light);
    gSPLight(renderState->dl++, light, 1);

    gSPMatrix(renderState->dl++, cubeMatrix, G_MTX_MODELVIEW | G_MTX_PUSH | G_MTX_MUL);

    for (int i = 0; i < NormalPassCount; ++i) {
        gSPDisplayList(renderState->dl++, brick_normal_pass_mat[i]);
        gSPDisplayList(renderState->dl++, brick_normal_pass_gfx[i]);
        gDPPipeSync(renderState->dl++);

        if (i == 0) {
            gDPSetRenderMode(renderState->dl++, RB_ZB_ADD_DECAL(1), RB_ZB_ADD_DECAL(2));
        }

        break;
    }
    
    gSPPopMatrix(renderState->dl++, G_MTX_MODELVIEW);
    
    // adjust lightness buffer
    gDPPipeSync(renderState->dl++);
    gSPGeometryMode(renderState->dl++, G_LIGHTING | G_SHADE, 0);
    gDPSetColorImage(renderState->dl++, G_IM_FMT_I, G_IM_SIZ_8b, SCREEN_WD, osVirtualToPhysical(lightnessCombineBuffer));
    gSPSegment(renderState->dl++, SOURCE_CB_SEGMENT, lightnessDrawBuffer);
    gSPDisplayList(renderState->dl++, gAdjustBrightnessRange);

    // combine buffers
    gDPPipeSync(renderState->dl++);
    gDPSetColorImage(renderState->dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, osVirtualToPhysical(task->framebuffer));
    gDPFillRectangle(renderState->dl++, 0, 0, SCREEN_WD-1, SCREEN_HT-1);
    gDPPipeSync(renderState->dl++);
    gSPSegment(renderState->dl++, SOURCE_CB_SEGMENT, lightnessDrawBuffer);
    gSPSegment(renderState->dl++, SOURCE_COLOR_SEGMENT, colorBuffer);

    gSPDisplayList(renderState->dl++, gCombineBuffers);

}