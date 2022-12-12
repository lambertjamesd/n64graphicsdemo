# gspF3DEX2_fifo Performance

Below is an incomplete table of the perofmrance for commands part of the F3DEX2 microcode

Keep in mind these test were running with an idle CPU. If the CPU and RCP are both accessing RAM these times will be slower.

| Command | Microseconds (1/1000 milliseconds) |
|---------|---------------------|
| gsSPNoOp | 0.225 |
| gsSPGeometryMode | 0.324 |
| gSPSetOtherMode(G_SETOTHERMODE_H) <br> gsDPPipelineMode <br> gsDPSetCycleType <br> gsDPSetTexturePersp <br> gsDPSetTextureDetail <br> gsDPSetTextureLOD <br> gsDPSetTextureLUT <br> gsDPSetTextureFilter <br> gsDPSetTextureConvert <br> gsDPSetCombineKey <br> gsDPSetColorDither <br> gsDPSetAlphaDither | 0.770 |
| gSPSetOtherMode(G_SETOTHERMODE_L) <br> gsDPSetAlphaCompare <br> gsDPSetDepthSource <br> gsDPSetRenderMode | 0.751 |
| gsDPSetCombineMode <br> gsDPSetCombineLERP | 0.395 |
| gsDPSetPrimColor <br> gsDPSetEnvColor <br> gsDPSetBlendColor <br> gsDPSetFogColor <br> | 0.395 |
| gsSPTexture | 0.305 |
| gsDPSetTextureImage | 0.698 |
| gsDPSetTile | 0.395 |
| gsDPSetTileSize | 0.394 |
| *gsDPTileSync | 0.385 |
| *gsDPLoadSync | 0.388 |
| *gsDPPipeSync | 0.388 |
| gsSPMatrix LOAD | 1.780 |
| gsSPMatrix MUL | 4.134 |
| gsSPPopMatrix | 1.939 |
| **gsDPLoadBlock | 0.00772 * n_bytes |

\* This is the mimimum time of the operation. A previous unfinished command could cause this command to stall for longer

\** How long this operation takes depends on how many bytes are copied

## gsDPLoadBlock examples

These values are based on raw data collected which was used to determine the microseconds per bytes above

| G_IM_FMT | G_IM_SIZ | size | total bytes | Microsecends |
|----------|----------|------|-------------|--------------|
| G_IM_FMT_RGBA | G_IM_SIZ_16b | 32x64 | 4096 | 31.606 |
| G_IM_FMT_RGBA | G_IM_SIZ_16b | 32x32 | 2048 | 15.028 |
| G_IM_FMT_RGBA | G_IM_SIZ_16b | 16x32 | 1024 | 6.767 |
| G_IM_FMT_RGBA | G_IM_SIZ_16b | 32x16 | 1024 | 6.767 |
| G_IM_FMT_RGBA | G_IM_SIZ_16b | 8x32 | 512 | 2.634 |
| G_IM_FMT_RGBA | G_IM_SIZ_16b | 32x8 | 512 | 2.634 |
| G_IM_FMT_I | G_IM_SIZ_4b | 64x128 | 4096 | 15.031 |
| G_IM_FMT_I | G_IM_SIZ_4b | 32x64 | 1024 | 6.805 |

## TODO

This is, for now, an incomplete list of RCP commands. Most notably missing is `gsSPVertex` and `gsSP1Triangle`. The speed of these two commands will depend on how the RCP is configured, the screen size of triangles, and if the triangles are fully or partially clipped. Because measuring this will require more complicated test code it is left off for now.
