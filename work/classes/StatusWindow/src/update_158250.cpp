#include "class.h"

extern "C" void TouchButton_setAlwaysPressed(void *btn, bool pressed);

// StatusWindow::update(int) -- scroll inertia + selected-tab button highlight.
extern "C" void StatusWindow_update(StatusWindow *self)
{
    // Velocity integration while not being dragged (+0x54 == drag flag).
    if (self->f_54 == 0) {
        float v = self->f_48 * self->f_4c;
        self->f_4c = v;
        // If |v| >= 1.0 keep scrolling: advance the integer scroll offset (+0x38).
        float mag = v > 0.0f ? v : -v;
        if (mag >= 1.0f) {
            self->f_38 = (float)(int)(v + (float)self->f_38);
            self->f_38 = (int)(v + (float)self->f_38);
        }
    }

    int off = self->f_38;
    if (off > 0) {
        // Overscrolled past the top: spring back.
        self->f_48 = 1.0f;
        self->f_4c = (float)(-off) * 0.5f;
    }

    int bottom = self->f_5c - self->f_58;
    if (bottom < 0) {
        if (off < bottom) {
            // Overscrolled past the bottom: spring back.
            self->f_48 = 1.0f;
            self->f_4c = (float)(bottom - off) * 0.5f;
        }
    } else {
        // Content fits: clamp to top.
        self->f_4c = 0.0f;
        self->f_38 = 0;
    }

    // Highlight the button matching the active tab index (+0x30).
    void **arr = (void **)self->f_4;
    for (unsigned int idx = 0; idx < *(unsigned int *)arr; idx++) {
        void *btn = ((void **)arr[1])[idx];
        TouchButton_setAlwaysPressed(btn, idx == self->f_30);
    }
}
