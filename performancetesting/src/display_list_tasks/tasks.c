#include "tasks.h"

Gfx gTaskNop[] = {
    gsSPEndDisplayList(),
};

struct Task gTasks[] = {
    {NULL, gTaskNop, "nop"},
};

int gTaskCount = sizeof(gTasks) / sizeof(*gTasks);