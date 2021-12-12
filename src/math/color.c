#include "color.h"
#include "mathf.h"

struct Coloru8 gColorBlack = {0, 0, 0, 255};
struct Coloru8 gColorWhite = {255, 255, 255, 255};
struct Coloru8 gHalfTransparentBlack = {0, 0, 0, 128};
struct Coloru8 gHalfTransparentWhite = {255, 255, 255, 128};

void colorU8Lerp(struct Coloru8* from, struct Coloru8* to, float lerp, struct Coloru8* output) {
    output->r = (unsigned char)(mathfLerp(from->r, to->r, lerp));
    output->g = (unsigned char)(mathfLerp(from->g, to->g, lerp));
    output->b = (unsigned char)(mathfLerp(from->b, to->b, lerp));
    output->a = (unsigned char)(mathfLerp(from->a, to->a, lerp));
}