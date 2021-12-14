#ifndef __POINT_LIGHT_H__
#define __POINT_LIGHT_H__

#include <ultra64.h>

#include "math/vector3.h"
#include "graphics/color.h"

struct PointLight {
    struct Vector3 position;
    struct Coloru8 color;
    float intensity;
};

struct PointLightableMesh {
    struct Vector3* vertexNormals;
    struct Vector3* vertexTangents;
    struct Vector3* vertexCoTangents;
    Vtx* inputVertices;
    Vtx* oututVertices;
    Gfx* drawCommand;
    unsigned vertexCount;
};

extern Light gLightBlack;

void pointLightInit(struct PointLight* pointLight, struct Vector3* position, struct Coloru8* color, float intensity);
void pointLightCalculateLight(struct PointLight* pointLight, struct Vector3* target, Light* output);

void pointLightableMeshInit(struct PointLightableMesh* mesh, Vtx* inputVertices, Gfx* drawCommand);
void pointLightableCalc(struct PointLightableMesh* mesh, struct PointLight* pointLight);

#endif