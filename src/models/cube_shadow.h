#ifndef _cube_shadow_H
#define _cube_shadow_H

#include <ultra64.h>
#include "math/transform.h"
#include "sk64/skelatool_clip.h"

extern Gfx cube_shadow_model_gfx[];
#define CUBE_SHADOW_SHADOWTOP_BONE 0
#define CUBE_SHADOW_SHADOWBOTTOM_BONE 1
#define CUBE_SHADOW_DEFAULT_BONES_COUNT 2
extern struct Transform cube_shadow_default_bones[];
extern unsigned short cube_shadow_bone_parent[];
extern struct SKAnimationHeader cube_shadow_animations[];

#endif