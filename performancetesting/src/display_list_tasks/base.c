#include "base.h"

Gfx gThirdRecursiveLayer[] = {
    gsSPDisplayList(TEST_TASK_SEGMENT << 24),
    gsSPDisplayList(TEST_TASK_SEGMENT << 24),
    gsSPDisplayList(TEST_TASK_SEGMENT << 24),
    gsSPDisplayList(TEST_TASK_SEGMENT << 24),
    gsSPDisplayList(TEST_TASK_SEGMENT << 24),
    gsSPDisplayList(TEST_TASK_SEGMENT << 24),
    gsSPDisplayList(TEST_TASK_SEGMENT << 24),
    gsSPDisplayList(TEST_TASK_SEGMENT << 24),
    gsSPDisplayList(TEST_TASK_SEGMENT << 24),
    gsSPDisplayList(TEST_TASK_SEGMENT << 24),
    gsSPEndDisplayList(),
};

Gfx gSecondRecursiveLayer[] = {
    gsSPDisplayList(&gThirdRecursiveLayer[0]),
    gsSPDisplayList(&gThirdRecursiveLayer[0]),
    gsSPDisplayList(&gThirdRecursiveLayer[0]),
    gsSPDisplayList(&gThirdRecursiveLayer[0]),
    gsSPDisplayList(&gThirdRecursiveLayer[0]),
    gsSPDisplayList(&gThirdRecursiveLayer[0]),
    gsSPDisplayList(&gThirdRecursiveLayer[0]),
    gsSPDisplayList(&gThirdRecursiveLayer[0]),
    gsSPDisplayList(&gThirdRecursiveLayer[0]),
    gsSPDisplayList(&gThirdRecursiveLayer[0]),
    gsSPEndDisplayList(),
};

Gfx gRecursiveTest[] = {
    gsSPDisplayList(&gSecondRecursiveLayer[0]),
    gsSPDisplayList(&gSecondRecursiveLayer[0]),
    gsSPDisplayList(&gSecondRecursiveLayer[0]),
    gsSPDisplayList(&gSecondRecursiveLayer[0]),
    gsSPDisplayList(&gSecondRecursiveLayer[0]),
    gsSPDisplayList(&gSecondRecursiveLayer[0]),
    gsSPDisplayList(&gSecondRecursiveLayer[0]),
    gsSPDisplayList(&gSecondRecursiveLayer[0]),
    gsSPDisplayList(&gSecondRecursiveLayer[0]),
    gsSPDisplayList(&gSecondRecursiveLayer[0]),
    gsSPEndDisplayList(),
};

