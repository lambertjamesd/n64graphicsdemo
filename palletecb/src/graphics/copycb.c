#include "copycb.h"

#include "graphics.h"

// group u16 to u64
// 0x([0-9a-f]{4}), 0x([0-9a-f]{4}), 0x([0-9a-f]{4}), 0x([0-9a-f]{4})
// 0x$1$2$3$4

u64 gFirePallete[] = {
    0x0001000100010001, 0x0801080108010801, 0x1001100110011801, 0x1801180118012001, 
    0x2001200120012801, 0x2801280130013001, 0x3001380138013801, 0x3801400140014001, 
    0x4001480148014801, 0x5001500150015001, 0x5801580160016801, 0x7001780180018801, 
    0x90019801a001a801, 0xb001b001b001b001, 0xb841b841b841b841, 0xb881b881b881c083, 

    0xc0c3c0c3c0c3c103, 0xc103c103c103c943, 0xc945c945c985c985, 0xc985c985d1c5d1c5, 
    0xd1c5d205d207d207, 0xd207da07da47da47, 0xda47da47da47da87, 0xda87da87da87da87, 
    0xda87e2c7e2c7e2c9, 0xe2c9e2c9e2c9e309, 0xe309e309e309e309, 0xe349eb49eb49eb49, 
    0xeb49eb49eb8beb8b, 0xeb8beb8beb8bf3cb, 0xf3cbf3cbf3cbf3cb, 0xf3cbf40bf40bf40b, 

    0xf40bf40bf40df44b, 0xf44bf44bf44bf44b, 0xf48bf48bf48bf48b, 0xf48bf4cbf4cbf4cb, 
    0xf4cbf4cbf50bf509, 0xf509f509f509f549, 0xf549f549f549f549, 0xf589f589f589f589, 
    0xf589f5c9f5c9f5c7, 0xf5c7f5c7f5c7f607, 0xf607f607f607f607, 0xf647f647f647f647, 
    0xf647f687f687f687, 0xf687f687f687f6c5, 0xf6c5f6c5f6c5f6c5, 0xf6c5f705f705f705, 

    0xf705f705f705f707, 0xf747f747f749f749, 0xf74bf74bf74bf74d, 0xf74df74ff74ff74f, 
    0xf751f751f751f753, 0xf753f755f755f757, 0xf757f757f759f759, 0xf759f75bf75bf75d, 
    0xf75df75df75ff75f, 0xf75ff761f761f763, 0xf763f763f765f765, 0xf765f767f767f767, 
    0xf769f769f76bf76b, 0xf76bf76df76df76d, 0xf76ff76ff76ff771, 0xf771f773f773f773
};

u64 gToonPallete[] = {
    0x18c718c718c718c7, 0x08cb08cb32573257, 0x0a550a5545674567, 0x62036203e615e615,
    0x70c570c5db91db91, 0x784f784fe1d5e1d5, 0x18c718c718c718c7, 0x18c718c718c718c7, 
    0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 
    0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 

    0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 
    0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 
    0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 
    0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 
    
    0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 
    0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 
    0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 
    0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 
    
    0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 
    0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 
    0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 
    0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 0xffffffffffffffff,
};

u64 gIcePallete[] = {
    0x000b000b000b000b, 0x000b000b000b000b, 0x000b000b000b000b, 0x004d004d004d004d, 
    0x004d004d004d004d, 0x004d004d004d004d, 0x004d004f004f008f, 0x004f008f008f008f, 
    0x008f008f088f008f, 0x088f088f088f088f, 0x0891089108910891, 0x08d108d108d108d1, 
    0x08d108d108d108d1, 0x08d108d308d308d3, 0x08d308d308d308d3, 0x08d3091309130913, 
    
    0x0913091309130915, 0x1115091509151115, 0x1115111511151115, 0x1115115511551157, 
    0x1157115711571157, 0x1157115711571157, 0x1157115711571157, 0x1197119711991199, 
    0x119919991999199b, 0x19db19db19db19dd, 0x19dd1a1d1a1d1a1f, 0x1a1f221f225f225f, 
    0x22612261226122a1, 0x22a322a322a322a3, 0x22e52ae52ae52ae5, 0x2ae52ae72b272b27, 
    
    0x2b272b272b272b27, 0x2b272b672b672b67, 0x2b67236923692369, 0x23a923a923a923a9, 
    0x23a923a923a923a9, 0x23eb23eb23eb23eb, 0x23eb23eb23eb23eb, 0x242b242b242b242b, 
    0x242b1c2b1c2d1c2d, 0x1c6d1c6d1c6d1c6d, 0x1c6d1c6d1c6d1cad, 0x1cad1caf1caf1caf, 
    0x1caf1caf1cef1cef, 0x1cef1cef1cef1cef, 0x1cef1cef1cef1d31, 0x1d31153115311531, 
    
    0x1531153115711571, 0x1571157115711573, 0x157315b315b315b3, 0x15b315b315b315f3, 
    0x15f315f315f50df5, 0x0df50df50df50e35, 0x0e350e350e350e35, 0x0e350e370e770e77, 
    0x0e770e770e770e77, 0x0eb70eb70eb70eb7, 0x06b706b906b906f9, 0x06f906f906f906f9, 
    0x06f906f907390739, 0x0739073b073b073b, 0x073b077b077b077b, 0x077b077b077b077d,
};

u64 gYesWeCan[] = {
    0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 
    0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 
    0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 
    0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 0x18c718c718c718c7, 
    
    0x0193019301930193, 0x0193019301930193, 0x0193019301930193, 0x0193019301930193, 
    0xc909c909c909c909, 0xc909c909c909c909, 0xc909c909c909c909, 0xc909c909c909c909, 
    0xc909c909c909c909, 0xc909c909c909c909, 0xc909c909c909c909, 0xc909c909c909c909, 
    0x74e974e974e974e9, 0x74e974e974e974e9, 0x74e974e974e974e9, 0x74e974e974e974e9, 
    
    0x74e974e974e974e9, 0x74e974e974e974e9, 0x74e974e974e974e9, 0x74e974e974e974e9, 
    0xbde7bde7bde7bde7, 0xbde7bde7bde7bde7, 0xbde7bde7bde7bde7, 0xbde7bde7bde7bde7, 
    0xf6e9f6e9f6e9f6e9, 0xf6e9f6e9f6e9f6e9, 0xf6e9f6e9f6e9f6e9, 0xf6e9f6e9f6e9f6e9, 
    0xf6e9f6e9f6e9f6e9, 0xf6e9f6e9f6e9f6e9, 0xf6e9f6e9f6e9f6e9, 0xf6e9f6e9f6e9f6e9, 
    
    0xf727f727f727f727, 0xf727f727f727f727, 0xf727f727f727f727, 0xf727f727f727f727, 
    0xf727f727f727f727, 0xf727f727f727f727, 0xf727f727f727f727, 0xf727f727f727f727, 
    0xf727f727f727f727, 0xf727f727f727f727, 0xf727f727f727f727, 0xf727f727f727f727, 
    0xf727f727f727f727, 0xf727f727f727f727, 0xf727f727f727f727, 0xf727f727f727f727,
};

u8 __attribute__((aligned(64))) indexColorBuffer[SCREEN_HT * SCREEN_WD];

#define UNLIT_TEXTURE   0, 0, 0, TEXEL0, 0, 0, 0, ENVIRONMENT

#define COPY_IMAGE_TILE(x, y, w, h)                             \
    gsDPLoadTextureTile(                                        \
        SOURCE_CB,                                              \
        G_IM_FMT_CI, G_IM_SIZ_8b,                               \
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

#define COPY_HALF_IMAGE_ROW(x, y, w, h)                         \
    COPY_IMAGE_TILE((x) + 0, y, w, h),                          \
    COPY_IMAGE_TILE((x) + 64, y, w, h),                         \
    COPY_IMAGE_TILE((x) + 128, y, w, h),                        \
    COPY_IMAGE_TILE((x) + 192, y, w, h),                        \
    COPY_IMAGE_TILE((x) + 256, y, w, h)

#define COPY_FULL_IMAGE_ROW(y, w, h)    COPY_HALF_IMAGE_ROW(0, y, w, h), COPY_HALF_IMAGE_ROW(320, y, w, h)

Gfx gCopyCB[] = {
    gsDPPipeSync(),
    gsDPSetCombineMode(UNLIT_TEXTURE, UNLIT_TEXTURE),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetEnvColor(255, 255, 255, 255),
    gsSPClearGeometryMode(G_ZBUFFER),
    gsSPTexture(0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON),


    COPY_FULL_IMAGE_ROW(0, 64, 32),
    COPY_FULL_IMAGE_ROW(32, 64, 32),
    COPY_FULL_IMAGE_ROW(64, 64, 32),
    COPY_FULL_IMAGE_ROW(96, 64, 32),
    COPY_FULL_IMAGE_ROW(128, 64, 32),
    COPY_FULL_IMAGE_ROW(160, 64, 32),
    COPY_FULL_IMAGE_ROW(192, 64, 32),
    COPY_FULL_IMAGE_ROW(224, 64, 32),
    COPY_FULL_IMAGE_ROW(256, 64, 32),
    COPY_FULL_IMAGE_ROW(288, 64, 32),
    COPY_FULL_IMAGE_ROW(320, 64, 32),
    COPY_FULL_IMAGE_ROW(352, 64, 32),
    COPY_FULL_IMAGE_ROW(384, 64, 32),
    COPY_FULL_IMAGE_ROW(416, 64, 32),
    COPY_FULL_IMAGE_ROW(448, 64, 32),
    
    gsSPEndDisplayList(),
};