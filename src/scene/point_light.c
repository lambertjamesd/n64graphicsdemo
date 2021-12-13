
#include "point_light.h"
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