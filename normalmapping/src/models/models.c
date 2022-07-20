
#include <ultra64.h>

#include "models.h"

#include "sphere_geo.inc.h"

#include "../../build/assets/brick_color.h"
#include "../../build/assets/brick_tangent.h"
#include "../../build/assets/brick_ntangent.h"
#include "../../build/assets/brick_bitangent.h"
#include "../../build/assets/brick_nbitangent.h"
#include "../../build/assets/brick_normal.h"

#include "../../build/assets/materials.h"

Gfx* brick_color_gfx = brick_color_model_gfx;
Gfx* brick_color_mat = materials_brick_color;


Gfx* brick_normal_pass_gfx[] = {
    [NormalPassX] = brick_tangent_model_gfx,
    [NormalPassNX] = brick_ntangent_model_gfx,
    [NormalPassY] = brick_bitangent_model_gfx,
    [NormalPassNY] = brick_nbitangent_model_gfx,
    [NormalPassZ] = brick_normal_model_gfx,
};

Gfx* brick_normal_pass_mat[] = {
    [NormalPassX] = materials_brick_tangent,
    [NormalPassNX] = materials_brick_ntangent,
    [NormalPassY] = materials_brick_bitangent,
    [NormalPassNY] = materials_brick_nbitangent,
    [NormalPassZ] = materials_brick_normal,
};