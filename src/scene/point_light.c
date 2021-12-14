
#include "point_light.h"
#include "defs.h"
#include "util/memory.h"
#include "math/mathf.h"
#include "defs.h"

Light gLightBlack = {{{0, 0, 0}, 0, {0, 0, 0}, 0, {0, 0x7f, 0}, 0}};

void pointLightInit(struct PointLight* pointLight, struct Vector3* position, struct Coloru8* color, float intensity) {
    pointLight->position = *position;
    pointLight->color = *color;
    pointLight->intensity = intensity;
}

void pointLightCalculateLight(struct PointLight* pointLight, struct Vector3* target, Light* output) {
    output->l.col[0] = pointLight->color.r;
    output->l.col[1] = pointLight->color.g;
    output->l.col[2] = pointLight->color.b;

    output->l.colc[0] = pointLight->color.r;
    output->l.colc[1] = pointLight->color.g;
    output->l.colc[2] = pointLight->color.b;

    struct Vector3 normalized;
    struct Vector3 offset;
    vector3Sub(&pointLight->position, target, &offset);
    vector3Normalize(&offset, &normalized);
    vector3ToVector3u8(&normalized, (struct Vector3u8*)output->l.dir);
}

#define MAX_REASONABLE_VERTICES     1000

#define GET_GFX_TYPE(gfx)       (_SHIFTR((gfx)->words.w0, 24, 8))

void pointLightableMeshInit(struct PointLightableMesh* mesh, Vtx* inputVertices, Gfx* drawCommand) {
    unsigned vertexCount = 0;
    // start at 1 for the gSPEndDisplayList at the end
    unsigned gfxCommandCount = 1;

    for (Gfx* curr = drawCommand; GET_GFX_TYPE(curr) != G_ENDDL; ++curr) {
        unsigned type = GET_GFX_TYPE(curr);

        switch (type) {
            case G_VTX:
            {
                unsigned base = (Vtx*)curr->words.w1 - inputVertices;
                base += _SHIFTR(curr->words.w0,12,8);
                vertexCount = MAX(vertexCount, base);
                break;
            }
        }

        ++gfxCommandCount;
    }

    mesh->vertexNormals = malloc(sizeof(struct Vector3) * vertexCount);
    mesh->vertexTangents = malloc(sizeof(struct Vector3) * vertexCount);
    mesh->vertexCoTangents = malloc(sizeof(struct Vector3) * vertexCount);
    mesh->oututVertices = malloc(sizeof(Vtx) * vertexCount);
    mesh->inputVertices = inputVertices;
    mesh->drawCommand = malloc(sizeof(Gfx) * gfxCommandCount);
    mesh->vertexCount = vertexCount;

    for (unsigned i = 0; i < vertexCount; ++i) {
        struct Vector3* normal = &mesh->vertexNormals[i];
        normal->x = inputVertices[i].n.n[0];
        normal->y = inputVertices[i].n.n[1];
        normal->z = inputVertices[i].n.n[2];
        vector3Normalize(normal, normal);

        float rightDot = vector3Dot(normal, &gRight);
        float forwardDot = vector3Dot(normal, &gUp);

        struct Vector3* tangent = &mesh->vertexTangents[i];

        if (fabsf(rightDot) < fabsf(forwardDot)) {
            vector3ProjectPlane(&gRight, normal, tangent);
        } else {
            vector3ProjectPlane(&gUp, normal, tangent);
        }
        vector3Normalize(tangent, tangent);

        mesh->oututVertices[i] = mesh->inputVertices[i];

        vector3Cross(normal, tangent, &mesh->vertexCoTangents[i]);
    }

    Gfx* currGfx = mesh->drawCommand;

    for (unsigned i = 0; i < gfxCommandCount; ++i) {
        *currGfx = drawCommand[i];
        if (GET_GFX_TYPE(currGfx) == G_VTX) {
            currGfx->words.w1 = (unsigned)(((Vtx*)currGfx->words.w1 - inputVertices) + mesh->oututVertices);
        }

        ++currGfx;
    }
}

#define RENDERED_LIGHT_HEIGHT       (0.5f * SCENE_SCALE)
#define RENDERED_LIGHT_TEX_SIZE     (4.0f * SCENE_SCALE)
#define RENDERED_LIGHT_TEX_UV_SIZE  4096

void pointLightableCalc(struct PointLightableMesh* mesh, struct PointLight* pointLight) {
    for (unsigned i = 0; i < mesh->vertexCount; ++i) {
        struct Vector3 vertexPos;
        Vtx_tn* input = &mesh->inputVertices[i].n;
        vertexPos.x = input->ob[0];
        vertexPos.y = input->ob[1];
        vertexPos.z = input->ob[2];

        struct Vector3 offset;
        vector3Sub(&pointLight->position, &vertexPos, &offset);

        float perpDistnace = vector3Dot(&mesh->vertexNormals[i], &offset);
        float tangentDistnace = vector3Dot(&mesh->vertexTangents[i], &offset);
        float coTangentDistnace = vector3Dot(&mesh->vertexCoTangents[i], &offset);

        float uvWorldSize = RENDERED_LIGHT_TEX_SIZE * perpDistnace / RENDERED_LIGHT_HEIGHT;

        float u = -(tangentDistnace / uvWorldSize - 0.5f) * RENDERED_LIGHT_TEX_UV_SIZE;
        float v = -(coTangentDistnace / uvWorldSize - 0.5f) * RENDERED_LIGHT_TEX_UV_SIZE;

        Vtx_t* output = &mesh->oututVertices[i].v;
        output->tc[0] = (short)u;
        output->tc[1] = (short)v;
    }
}