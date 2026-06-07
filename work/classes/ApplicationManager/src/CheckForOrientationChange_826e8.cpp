#include "class.h"

extern "C" int g_orientationLeft;
extern "C" int g_orientationFlat;
extern "C" int g_orientationRight;
extern "C" int g_orientationUpsideDown;
extern "C" int g_orientationInactive;
extern "C" void PaintCanvas_SetGameOrientation(void *canvas, int orientation);

static bool update_orientation_timer(ApplicationManager *self, int *timer)
{
    int elapsed = *(int *)((char *)self + 0x70) - *(int *)((char *)self + 0x78);
    int value = *timer + elapsed;
    *timer = value;
    return value >= 0xfb;
}

extern "C" void ApplicationManager_CheckForOrientationChange(ApplicationManager *self)
{
    void *engine = *(void **)((char *)self + 0xa8);
    double tilt = *(double *)((char *)engine + 0x4b0);
    void *canvas;
    int orientation;
    int *timer;
    int target;

    if (tilt < -0.5) {
        canvas = *(void **)self;
        orientation = *(int *)((char *)canvas + 0x30);
        if (orientation == 0) {
            timer = &g_orientationLeft;
            target = 1;
            if (update_orientation_timer(self, timer)) {
                goto setOrientation;
            }
            return;
        }
    }

    if (tilt > 0.5) {
        canvas = *(void **)self;
        orientation = *(int *)((char *)canvas + 0x30);
        if (orientation == 3) {
            timer = &g_orientationRight;
            target = 2;
            if (update_orientation_timer(self, timer)) {
                goto setOrientation;
            }
            return;
        }
        if (orientation == 1) {
            timer = &g_orientationFlat;
            if (update_orientation_timer(self, timer)) {
                PaintCanvas_SetGameOrientation(canvas, 0);
                *timer = 0;
            }
            return;
        }
    }

    if (tilt < -0.5) {
        canvas = *(void **)self;
        if (*(int *)((char *)canvas + 0x30) == 2) {
            timer = &g_orientationUpsideDown;
            target = 3;
            if (update_orientation_timer(self, timer)) {
                goto setOrientation;
            }
            return;
        }
    }

    g_orientationInactive = 0;
    return;

setOrientation:
    PaintCanvas_SetGameOrientation(canvas, target);
    *timer = 0;
}
