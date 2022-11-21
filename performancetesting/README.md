# gspF3DEX2_fifo Performance

Below is an incomplete table of the perofmrance for commands part of the F3DEX2 microcode

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

This is, for now, an incomplete list of RCP commands. Most notably missing is `gsSPVertex` and `gsSP1Triangle`. The speed of these two commands will depend on how the RCP is configured, the screen size of triangles, and if the triangles are fully or partially clipped. Because measuring this will require more complicated test code it is left off for now.