#include "base.h"

Gfx gTenTimes[] = {
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

Gfx gHundredTimes[] = {
    gsSPDisplayList(&gTenTimes[0]),
    gsSPDisplayList(&gTenTimes[0]),
    gsSPDisplayList(&gTenTimes[0]),
    gsSPDisplayList(&gTenTimes[0]),
    gsSPDisplayList(&gTenTimes[0]),
    gsSPDisplayList(&gTenTimes[0]),
    gsSPDisplayList(&gTenTimes[0]),
    gsSPDisplayList(&gTenTimes[0]),
    gsSPDisplayList(&gTenTimes[0]),
    gsSPDisplayList(&gTenTimes[0]),
    gsSPEndDisplayList(),
};

Gfx gThousandTimes[] = {
    gsSPDisplayList(&gHundredTimes[0]),
    gsSPDisplayList(&gHundredTimes[0]),
    gsSPDisplayList(&gHundredTimes[0]),
    gsSPDisplayList(&gHundredTimes[0]),
    gsSPDisplayList(&gHundredTimes[0]),
    gsSPDisplayList(&gHundredTimes[0]),
    gsSPDisplayList(&gHundredTimes[0]),
    gsSPDisplayList(&gHundredTimes[0]),
    gsSPDisplayList(&gHundredTimes[0]),
    gsSPDisplayList(&gHundredTimes[0]),
    gsSPEndDisplayList(),
};



Gfx gTenThousandTimes[] = {
    gsSPDisplayList(&gThousandTimes[0]),
    gsSPDisplayList(&gThousandTimes[0]),
    gsSPDisplayList(&gThousandTimes[0]),
    gsSPDisplayList(&gThousandTimes[0]),
    gsSPDisplayList(&gThousandTimes[0]),
    gsSPDisplayList(&gThousandTimes[0]),
    gsSPDisplayList(&gThousandTimes[0]),
    gsSPDisplayList(&gThousandTimes[0]),
    gsSPDisplayList(&gThousandTimes[0]),
    gsSPDisplayList(&gThousandTimes[0]),
    gsSPEndDisplayList(),
};

Gfx gHundredThousandTimes[] = {
    gsSPDisplayList(&gTenThousandTimes[0]),
    gsSPDisplayList(&gTenThousandTimes[0]),
    gsSPDisplayList(&gTenThousandTimes[0]),
    gsSPDisplayList(&gTenThousandTimes[0]),
    gsSPDisplayList(&gTenThousandTimes[0]),
    gsSPDisplayList(&gTenThousandTimes[0]),
    gsSPDisplayList(&gTenThousandTimes[0]),
    gsSPDisplayList(&gTenThousandTimes[0]),
    gsSPDisplayList(&gTenThousandTimes[0]),
    gsSPDisplayList(&gTenThousandTimes[0]),
    gsSPEndDisplayList(),
};