#ifndef __MODELS_H__
#define __MODELS_H__

#include "sphere.h"

extern Gfx* brick_color_gfx;
extern Gfx* brick_color_mat;

extern Gfx* brick_normal_pass_gfx[];
extern Gfx* brick_normal_pass_mat[];

enum NormalPass {
    NormalPassX,
    NormalPassNX,
    NormalPassY,
    NormalPassNY,
    NormalPassZ,
    NormalPassCount,
};

#endif