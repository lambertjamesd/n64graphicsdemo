#include "copycb.h"

#include "graphics.h"

// group u16 to u64
// 0x([0-9a-f]{4}), 0x([0-9a-f]{4}), 0x([0-9a-f]{4}), 0x([0-9a-f]{4})
// 0x$1$2$3$4

u64 gPalleteMapping[] = {
    0x0897089710971097, 0x1097189718971897, 0x1897209720972097, 0x2897289728972897, 
    0x3097309730973897, 0x3897389740974097, 0x4097409748974897, 0x4897509750975097, 
    0x58975897589758d7, 0x60d76097609768d7, 0x68d768d770d770d7, 0x70d778d778d778d7, 
    0x78d780d780d580d5, 0x88d588d588d588d5, 0x90d590d590d598d5, 0x8113811381138113, 
    
    0x8913891389159115, 0x911590d590d598d5, 0x98d598d5a097a097, 0xa095a095a895a895, 
    0xa895a893a893b0d3, 0xb093b0d3b0d3b0d1, 0xb0d1b8d1b8d1b8d1, 0xb8d1b8cfb8cfc0cf, 
    0xc0cfc0cfc0cfc0cd, 0xc8cdc8cdc8cdc8cd, 0xc8cdc8cbd0cbd0cb, 0xd0cbd0cbd0c9d8c9, 
    0xd8c9d8c9d909d909, 0xd907d907e107e107, 0xe107e107e105e905, 0xe905e905e905e905, 
    
    0xe903e943e943e943, 0xe983e983e983e9c3, 0xe9c3e9c3ea03f203, 0xf203f243f243f243, 
    0xf283f283f283f2c3, 0xf2c3f2c3f2c3f303, 0xf303f303f343f343, 0xf343f383f383f383, 
    0xf3c3f3c3f3c3f403, 0xf403f403f441f441, 0xf441f481f481f481, 0xf4c1f4c1f4c1f501, 
    0xf501f501f541f541, 0xf541f581f581f581, 0xf5c1f5c1f5c1f5c1, 0xf601f601f601f641, 
    
    0xf641f641f681f681, 0xf681f6c1f6c1f6c1, 0xf701f701f701f741, 0xf741f741f741f783, 
    0xf785f785f787f787, 0xf789f789f78bf78d, 0xf78df78ff791f791, 0xf793f793f795f797, 
    0xf797f799f799f79b, 0xf79df79df79ff79f, 0xf7a1f7a3f7a3f7a5, 0xf7a5f7a7f7a7f7a9, 
    0xf7abf7abf7adf7af, 0xf7aff7b1f7b1f7b3, 0xf7b5f7b5f7b7f7b7, 0xf7b9f7bbf7bbf7bd,
};

u8 __attribute__((aligned(64))) lightnessDrawBuffer[SCREEN_HT * SCREEN_WD];
u8 __attribute__((aligned(64))) lightnessCombineBuffer[SCREEN_HT * SCREEN_WD];

u16 __attribute__((aligned(64))) colorBuffer[SCREEN_HT * SCREEN_WD];

#define COPY_IMAGE_TILE(x, y, w, h, imfmt)                      \
    gsDPLoadTextureTile(                                        \
        SOURCE_CB,                                              \
        imfmt, G_IM_SIZ_8b,                                     \
        SCREEN_WD, SCREEN_HT,                                   \
        (x), (y), (x) + (w) - 1, (y) + (h) - 1,                 \
        0,                                                      \
        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, \
        G_TX_NOMASK, G_TX_NOMASK,                               \
        G_TX_NOLOD, G_TX_NOLOD                                  \
    ),                                                          \
    gsSPTextureRectangle(                                       \
        (x) << 2, (y) << 2,                                     \
        ((x) + (w)) << 2, ((y) + (h)) << 2,                     \
        G_TX_RENDERTILE,                                        \
        (x) << 5, (y) << 5,                                     \
        1 << 10, 1 << 10                                        \
    )

#define COPY_HALF_IMAGE_ROW(x, y, w, h, imfmt)                  \
    COPY_IMAGE_TILE((x) + 0, y, w, h, imfmt),                   \
    COPY_IMAGE_TILE((x) + 64, y, w, h, imfmt),                  \
    COPY_IMAGE_TILE((x) + 128, y, w, h, imfmt),                 \
    COPY_IMAGE_TILE((x) + 192, y, w, h, imfmt),                 \
    COPY_IMAGE_TILE((x) + 256, y, w, h, imfmt)

#define COPY_FULL_IMAGE_ROW(y, w, h, imfmt)    COPY_HALF_IMAGE_ROW(0, y, w, h, imfmt)


#define COMBINE_IMAGE_TILE(x, y, w, h)                          \
    gsDPLoadMultiTile(                                          \
        SOURCE_COLOR,                                           \
        0,                                                      \
        G_TX_RENDERTILE + 0,                                    \
        G_IM_FMT_RGBA, G_IM_SIZ_16b,                            \
        SCREEN_WD, SCREEN_HT,                                   \
        (x), (y), (x) + (w) - 1, (y) + (h) - 1,                 \
        0,                                                      \
        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, \
        G_TX_NOMASK, G_TX_NOMASK,                               \
        G_TX_NOLOD, G_TX_NOLOD                                  \
    ),                                                          \
    gsDPLoadMultiTile(                                          \
        SOURCE_CB,                                              \
        256,                                                    \
        G_TX_RENDERTILE + 1,                                    \
        G_IM_FMT_I, G_IM_SIZ_8b,                                \
        SCREEN_WD, SCREEN_HT,                                   \
        (x), (y), (x) + (w) - 1, (y) + (h) - 1,                 \
        0,                                                      \
        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, \
        G_TX_NOMASK, G_TX_NOMASK,                               \
        G_TX_NOLOD, G_TX_NOLOD                                  \
    ),                                                          \
    gsSPTextureRectangle(                                       \
        (x) << 2, (y) << 2,                                     \
        ((x) + (w)) << 2, ((y) + (h)) << 2,                     \
        G_TX_RENDERTILE,                                        \
        (x) << 5, (y) << 5,                                     \
        1 << 10, 1 << 10                                        \
    )

#define COMBINE_HALF_IMAGE_ROW(x, y, w, h)                      \
    COMBINE_IMAGE_TILE((x) + 0, y, w, h),                       \
    COMBINE_IMAGE_TILE((x) + 64, y, w, h),                      \
    COMBINE_IMAGE_TILE((x) + 128, y, w, h),                     \
    COMBINE_IMAGE_TILE((x) + 192, y, w, h),                     \
    COMBINE_IMAGE_TILE((x) + 256, y, w, h)

#define COMBINE_FULL_IMAGE_ROW(y, w, h)    COMBINE_HALF_IMAGE_ROW(0, y, w, h)

#define LOAD_TEXTURE_COLOR       1, 0, TEXEL1, 0, 0, 0, 0, ENVIRONMENT
#define MULTIPLY_OTHER_TEXTURE   TEXEL1, 0, COMBINED, 0, 0, 0, 0, ENVIRONMENT

Gfx gCombineBuffers[] = {
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_2CYCLE),
    gsDPSetCombineMode(LOAD_TEXTURE_COLOR, MULTIPLY_OTHER_TEXTURE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetRenderMode(G_RM_PASS, G_RM_OPA_SURF2),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetEnvColor(64, 64, 64, 255),
    gsSPClearGeometryMode(G_ZBUFFER),
    gsSPTexture(0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON),


    COMBINE_FULL_IMAGE_ROW(0, 64, 16),
    COMBINE_FULL_IMAGE_ROW(16, 64, 16),
    COMBINE_FULL_IMAGE_ROW(32, 64, 16),
    COMBINE_FULL_IMAGE_ROW(48, 64, 16),
    COMBINE_FULL_IMAGE_ROW(64, 64, 16),
    COMBINE_FULL_IMAGE_ROW(80, 64, 16),
    COMBINE_FULL_IMAGE_ROW(96, 64, 16),
    COMBINE_FULL_IMAGE_ROW(112, 64, 16),
    COMBINE_FULL_IMAGE_ROW(128, 64, 16),
    COMBINE_FULL_IMAGE_ROW(144, 64, 16),
    COMBINE_FULL_IMAGE_ROW(160, 64, 16),
    COMBINE_FULL_IMAGE_ROW(176, 64, 16),
    COMBINE_FULL_IMAGE_ROW(192, 64, 16),
    COMBINE_FULL_IMAGE_ROW(208, 64, 16),
    COMBINE_FULL_IMAGE_ROW(224, 64, 32),

    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
    
    gsSPEndDisplayList(),
};

#define CLAMP_HALF_RANGE   TEXEL0, PRIMITIVE, PRIMITIVE_ALPHA, 0, 0, 0, 0, ENVIRONMENT

Gfx gAdjustBrightnessRange[] = {
    gsDPPipeSync(),
    gsDPSetCombineMode(CLAMP_HALF_RANGE, CLAMP_HALF_RANGE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetEnvColor(255, 255, 255, 255),
    gsDPSetPrimColor(255, 255, 128, 128, 128, 255),
    gsSPClearGeometryMode(G_ZBUFFER),
    gsSPTexture(0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON),


    COPY_FULL_IMAGE_ROW(0, 64, 32, G_IM_FMT_I),
    COPY_FULL_IMAGE_ROW(32, 64, 32, G_IM_FMT_I),
    COPY_FULL_IMAGE_ROW(64, 64, 32, G_IM_FMT_I),
    COPY_FULL_IMAGE_ROW(96, 64, 32, G_IM_FMT_I),
    COPY_FULL_IMAGE_ROW(128, 64, 32, G_IM_FMT_I),
    COPY_FULL_IMAGE_ROW(160, 64, 32, G_IM_FMT_I),
    COPY_FULL_IMAGE_ROW(192, 64, 32, G_IM_FMT_I),
    COPY_FULL_IMAGE_ROW(224, 64, 32, G_IM_FMT_I),
    
    gsSPEndDisplayList(),
};