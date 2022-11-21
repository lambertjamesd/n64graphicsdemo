#include "tasks.h"

#include "base.h"

Gfx gTaskEmpty[] = {
    gsSPEndDisplayList(),
};

Gfx gTaskNop[] = {
    gsSPNoOp(),
    gsSPEndDisplayList(),
};

Gfx gTaskGeometryMode[] = {
    gsSPGeometryMode(G_CULL_FRONT, G_CULL_BACK | G_LIGHTING),
    gsSPEndDisplayList(),
};

Gfx gTaskPipleineMode[] = {
    gsDPPipelineMode(G_PM_NPRIMITIVE),
    gsSPEndDisplayList(),
};

Gfx gTaskCycleType[] = {
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPEndDisplayList(),
};

Gfx gTaskTextureFilter[] = {
    gsDPSetTextureFilter(G_TF_POINT),
    gsSPEndDisplayList(),
};

Gfx gTaskAlphaCompare[] = {
    gsDPSetAlphaCompare(G_AC_DITHER),
    gsSPEndDisplayList(),
};

Gfx gTaskCombineMode[] = {
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPEndDisplayList(),
};

Gfx gTaskRenderMode[] = {
    gsDPSetRenderMode(G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2),
    gsSPEndDisplayList(),
};

Gfx gTaskPrimitiveColor[] = {
    gsDPSetPrimColor(255, 255, 255, 255, 255, 255),
    gsSPEndDisplayList(),
};

Gfx gTaskEnvColor[] = {
    gsDPSetEnvColor(255, 255, 255, 255),
    gsSPEndDisplayList(),
};

Gfx gTaskFogColor[] = {
    gsDPSetFogColor(255, 255, 255, 255),
    gsSPEndDisplayList(),
};

Gfx gTaskBlendColor[] = {
    gsDPSetBlendColor(255, 255, 255, 255),
    gsSPEndDisplayList(),
};

u64 gTileData[512];

Gfx gSetupTextureTests[] = {
    gsSPTexture(0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON),
    gsDPTileSync(),
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, gTileData),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 1023, 256),
    gsDPPipeSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
    gsDPSetTileSize(0, 0, 0, 124, 124),
    gsSPEndDisplayList(),
};

Gfx gSetup4bTextureTests[] = {
    gsSPTexture(0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON),
    gsDPTileSync(),
    gsDPSetTextureImage(G_IM_FMT_I, G_IM_SIZ_4b_LOAD_BLOCK, 1, gTileData),
    gsDPSetTile(G_IM_FMT_I, G_IM_SIZ_4b_LOAD_BLOCK, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 6, 0),
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 2047, 512),
    gsDPPipeSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
    gsDPSetTileSize(0, 0, 0, 124, 124),
    gsSPEndDisplayList(),
};

Gfx gTaskTexture[] = {
    gsSPTexture(0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};

Gfx gTaskTileSync[] = {
    gsDPTileSync(),
    gsSPEndDisplayList(),
};

Gfx gTaskSetTextureImage[] = {
    gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, gTileData),
    gsSPEndDisplayList(),
};

Gfx gTaskSetTile[] = {
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
    gsSPEndDisplayList(),
};

Gfx gTaskLoadSync[] = {
    gsDPLoadSync(),
    gsSPEndDisplayList(),
};

Gfx gTaskLoadBlock_rgba_16_32_64[] = {
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 2047, 256),
    gsSPEndDisplayList(),
};

Gfx gTaskLoadBlock_rgba_16_32_32[] = {
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 1023, 256),
    gsSPEndDisplayList(),
};

Gfx gTaskLoadBlock_rgba_16_16_32[] = {
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 511, 512),
    gsSPEndDisplayList(),
};

Gfx gTaskLoadBlock_rgba_16_8_32[] = {
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 255, 1024),
    gsSPEndDisplayList(),
};

Gfx gTaskLoadBlock_rgba_16_32_16[] = {
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 511, 256),
    gsSPEndDisplayList(),
};

Gfx gTaskLoadBlock_rgba_16_32_8[] = {
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 255, 256),
    gsSPEndDisplayList(),
};

Gfx gTaskLoadBlock_i_4_64_128[] = {
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 2047, 512),
    gsSPEndDisplayList(),
};

Gfx gTaskLoadBlock_i_4_64_64[] = {
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 1023, 512),
    gsSPEndDisplayList(),
};

Gfx gTaskLoadBlock_i_4_32_64[] = {
    gsDPLoadSync(),
    gsDPLoadBlock(G_TX_LOADTILE, 0, 0, 511, 1024),
    gsSPEndDisplayList(),
};

Gfx gTaskPipeSync[] = {
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

Gfx gTaskSetTileSize[] = {
    gsDPSetTileSize(0, 0, 0, 124, 124),
    gsSPEndDisplayList(),
};

Mtx gLoadMatrix;
Mtx gPerspMatrix;

Gfx gSetupMatrix[] = {
    gsSPMatrix(&gLoadMatrix, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH),
    gsSPMatrix(&gPerspMatrix, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH),
    gsSPEndDisplayList(),
};

Gfx gTaskMatrixLoadNoPush[] = {
    gsSPMatrix(&gLoadMatrix, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH),
    gsSPEndDisplayList(),
};

Gfx gTaskMatrixLoadPush[] = {
    gsSPMatrix(&gLoadMatrix, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_PUSH),
    gsSPPopMatrix(G_MTX_MODELVIEW),
    gsSPEndDisplayList(),
};

Gfx gTaskMatrixMulPush[] = {
    gsSPMatrix(&gLoadMatrix, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH),
    gsSPPopMatrix(G_MTX_MODELVIEW),
    gsSPEndDisplayList(),
};

struct Task gTasks[] = {
    {NULL, gTaskEmpty, gHundredThousandTimes, "<empty>"},
    {NULL, gTaskNop, gHundredThousandTimes, "gsSPNoOp"},
    {NULL, gTaskGeometryMode, gHundredThousandTimes, "gsSPGeometryMode"},
    {NULL, gTaskPipleineMode, gHundredThousandTimes, "gsDPPipelineMode"},
    {NULL, gTaskCycleType, gHundredThousandTimes, "gTaskCycleType"},
    {NULL, gTaskTextureFilter, gHundredThousandTimes, "gsDPSetTextureFilter"},
    {NULL, gTaskAlphaCompare, gHundredThousandTimes, "gsDPSetAlphaCompare"},
    {NULL, gTaskCombineMode, gHundredThousandTimes, "gsDPSetCombineMode"},
    {NULL, gTaskRenderMode, gHundredThousandTimes, "gsDPSetRenderMode"},
    {NULL, gTaskPrimitiveColor, gHundredThousandTimes, "gsDPSetPrimColor"},
    {NULL, gTaskEnvColor, gHundredThousandTimes, "gsDPSetEnvColor"},
    {NULL, gTaskFogColor, gHundredThousandTimes, "gsDPSetFogColor"},
    {NULL, gTaskBlendColor, gHundredThousandTimes, "gsDPSetBlendColor"},

    {gSetupTextureTests, gTaskTexture, gHundredThousandTimes, "gsSPTexture"},
    {gSetupTextureTests, gTaskTileSync, gHundredThousandTimes, "gsDPTileSync"},
    {gSetupTextureTests, gTaskSetTextureImage, gHundredThousandTimes, "gsDPSetTextureImage"},
    {gSetupTextureTests, gTaskSetTile, gHundredThousandTimes, "gsDPSetTile"},
    {gSetupTextureTests, gTaskLoadSync, gHundredThousandTimes, "gsDPLoadSync"},
    {gSetupTextureTests, gTaskLoadBlock_rgba_16_32_64, gHundredThousandTimes, "gsDPLoadBlock rgba 16 32x64"},
    {gSetupTextureTests, gTaskLoadBlock_rgba_16_32_32, gHundredThousandTimes, "gsDPLoadBlock rgba 16 32x32"},
    {gSetupTextureTests, gTaskLoadBlock_rgba_16_16_32, gHundredThousandTimes, "gsDPLoadBlock rgba 16 16x32"},
    {gSetupTextureTests, gTaskLoadBlock_rgba_16_8_32, gHundredThousandTimes, "gsDPLoadBlock rgba 16 8x32"},
    {gSetupTextureTests, gTaskLoadBlock_rgba_16_32_16, gHundredThousandTimes, "gsDPLoadBlock rgba 16 32x16"},
    {gSetupTextureTests, gTaskLoadBlock_rgba_16_32_8, gHundredThousandTimes, "gsDPLoadBlock rgba 16 32x8"},
    {gSetupTextureTests, gTaskPipeSync, gHundredThousandTimes, "gsDPPipeSync"},
    {gSetupTextureTests, gTaskSetTileSize, gHundredThousandTimes, "gsDPSetTileSize"},

    {gSetup4bTextureTests, gTaskLoadBlock_i_4_64_128, gHundredThousandTimes, "gsDPLoadBlock i 4 64x128"},
    {gSetup4bTextureTests, gTaskLoadBlock_i_4_64_64, gHundredThousandTimes, "gsDPLoadBlock i 4 64x64"},
    {gSetup4bTextureTests, gTaskLoadBlock_i_4_32_64, gHundredThousandTimes, "gsDPLoadBlock i 4 32x64"},

    {gSetupMatrix, gTaskMatrixLoadNoPush, gHundredThousandTimes, "gsSPMatrix G_MTX_LOAD | G_MTX_NOPUSH"},
    {gSetupMatrix, gTaskMatrixLoadPush, gHundredThousandTimes, "gsSPMatrix G_MTX_LOAD | G_MTX_PUSH"},
    {gSetupMatrix, gTaskMatrixMulPush, gHundredThousandTimes, "gsSPMatrix G_MTX_MUL | G_MTX_PUSH"},
};

int gTaskCount = sizeof(gTasks) / sizeof(*gTasks);