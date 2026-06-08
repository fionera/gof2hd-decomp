#include "class.h"

extern "C" void TouchButton_setAlwaysPressed(void *btn, bool pressed);

// StatusWindow::update(int) -- scroll inertia + selected-tab button highlight.
extern "C" void StatusWindow_update(StatusWindow *self)
{
    // Velocity integration while not being dragged (+0x54 == drag flag).
    if (F<char>(self, 0x54) == 0) {
        float v = f32(self, 0x48) * f32(self, 0x4c);
        f32(self, 0x4c) = v;
        // If |v| >= 1.0 keep scrolling: advance the integer scroll offset (+0x38).
        float mag = v > 0.0f ? v : -v;
        if (mag >= 1.0f) {
            f32(self, 0x38) = (float)(int)(v + (float)i32(self, 0x38));
            i32(self, 0x38) = (int)(v + (float)i32(self, 0x38));
        }
    }

    int off = i32(self, 0x38);
    if (off > 0) {
        // Overscrolled past the top: spring back.
        f32(self, 0x48) = 1.0f;
        f32(self, 0x4c) = (float)(-off) * 0.5f;
    }

    int bottom = i32(self, 0x5c) - i32(self, 0x58);
    if (bottom < 0) {
        if (off < bottom) {
            // Overscrolled past the bottom: spring back.
            f32(self, 0x48) = 1.0f;
            f32(self, 0x4c) = (float)(bottom - off) * 0.5f;
        }
    } else {
        // Content fits: clamp to top.
        f32(self, 0x4c) = 0.0f;
        i32(self, 0x38) = 0;
    }

    // Highlight the button matching the active tab index (+0x30).
    void **arr = (void **)pp(self, 0x4);
    for (unsigned int idx = 0; idx < *(unsigned int *)arr; idx++) {
        void *btn = ((void **)arr[1])[idx];
        TouchButton_setAlwaysPressed(btn, idx == u32(self, 0x30));
    }
}
