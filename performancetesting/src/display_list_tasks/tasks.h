#ifndef __DISPLAY_LIST_TASKS_TASKS_H__
#define __DISPLAY_LIST_TASKS_TASKS_H__

#include <ultra64.h>

struct Task {
    Gfx* setupTask;
    Gfx* measuredTask;
    Gfx* testRunner;
    const char* name;
};

extern struct Task gTasks[];
extern int gTaskCount;

extern Mtx gLoadMatrix;
extern Mtx gPerspMatrix;

#endif