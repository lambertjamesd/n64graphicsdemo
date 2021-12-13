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

extern Light gLightBlack;

void pointLightInit(struct PointLight* pointLight, struct Vector3* position, struct Coloru8* color, float intensity);
void pointLightCalculateLight(struct PointLight* pointLight, struct Vector3* target, Light* output);

#endif