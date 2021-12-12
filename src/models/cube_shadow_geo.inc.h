#include "sk64/skelatool_defs.h"

Vtx cube_shadow_Cube_normal[] = {
    {{{100, 0, 99}, 0, {1280, 1024}, {0, 0, 127, 255}}},
    {{{-100, 0, 99}, 0, {1792, 1024}, {0, 0, 127, 255}}},
    {{{-100, 0, 100}, 0, {1792, 512}, {0, 0, 127, 255}}},
    {{{100, 0, 100}, 0, {1280, 512}, {0, 0, 127, 255}}},
    {{{-100, 0, -100}, 0, {256, 1024}, {0, 0, -128, 255}}},
    {{{100, 0, -100}, 0, {768, 1024}, {0, 0, -128, 255}}},
    {{{100, 0, -99}, 0, {768, 512}, {0, 0, -128, 255}}},
    {{{-100, 0, -99}, 0, {256, 512}, {0, 0, -128, 255}}},
    {{{100, 0, -100}, 0, {768, 1024}, {127, 0, 0, 255}}},
    {{{100, 0, 99}, 0, {1280, 1024}, {127, 0, 0, 255}}},
    {{{100, 0, 100}, 0, {1280, 512}, {127, 0, 0, 255}}},
    {{{100, 0, -99}, 0, {768, 512}, {127, 0, 0, 255}}},
};


Vtx cube_shadow_Cube_normal_1[] = {
    {{{-100, 0, -99}, 0, {768, 2048}, {-128, 0, 0, 255}}},
    {{{-100, 0, 100}, 0, {1280, 2048}, {-128, 0, 0, 255}}},
    {{{-100, 0, 99}, 0, {1280, 1536}, {-128, 0, 0, 255}}},
    {{{-100, 0, -100}, 0, {768, 1536}, {-128, 0, 0, 255}}},
};


Vtx cube_shadow_Cube_normal_2[] = {
    {{{100, 0, -99}, 0, {768, 512}, {0, -128, 0, 255}}},
    {{{100, 0, 100}, 0, {1280, 512}, {0, -128, 0, 255}}},
    {{{-100, 0, 100}, 0, {1280, 0}, {0, -128, 0, 255}}},
    {{{-100, 0, -99}, 0, {768, 0}, {0, -128, 0, 255}}},
};


Gfx cube_shadow_model_gfx[] = {
    // Material Material
    // End Material Material
    // ShadowBottom
    gsSPMatrix((Mtx*)MATRIX_TRANSFORM_SEGMENT_ADDRESS + 1, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH),
    gsSPVertex(&cube_shadow_Cube_normal[2], 2, 0),
    gsSPVertex(&cube_shadow_Cube_normal[6], 2, 2),
    gsSPVertex(&cube_shadow_Cube_normal[10], 2, 4),
    gsSPPopMatrix(G_MTX_MODELVIEW),
    // ShadowTop
    gsSPMatrix((Mtx*)MATRIX_TRANSFORM_SEGMENT_ADDRESS + 0, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH),
    gsSPVertex(&cube_shadow_Cube_normal[0], 2, 6),
    gsSPVertex(&cube_shadow_Cube_normal[4], 2, 8),
    gsSPVertex(&cube_shadow_Cube_normal[8], 2, 10),
    gsSP2Triangles(6, 7, 0, 0, 6, 0, 1, 0),
    gsSP2Triangles(8, 9, 2, 0, 8, 2, 3, 0),
    gsSP2Triangles(10, 11, 4, 0, 10, 4, 5, 0),
    // Material Material
    // End Material Material
    gsSPVertex(&cube_shadow_Cube_normal_1[2], 2, 0),
    gsSPPopMatrix(G_MTX_MODELVIEW),
    // ShadowBottom
    gsSPMatrix((Mtx*)MATRIX_TRANSFORM_SEGMENT_ADDRESS + 1, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH),
    gsSPVertex(&cube_shadow_Cube_normal_1[0], 2, 2),
    gsSP2Triangles(2, 3, 0, 0, 2, 0, 1, 0),
    // Material Material
    // End Material Material
    gsSPVertex(&cube_shadow_Cube_normal_2[0], 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPPopMatrix(G_MTX_MODELVIEW),
    gsSPEndDisplayList(),
};
