#include "class.h"

extern "C" void WantedWindow_update_tail(void *starMap, int dt);
extern "C" void TouchButton_setAlwaysPressed(void *button, bool pressed);
extern "C" void ScrollTouchWindow_update(void *window, int dt);

extern "C" void WantedWindow_update(WantedWindow *self, int dt)
{
    if (F<uint8_t>(self, 0x14) != 0) {
        return WantedWindow_update_tail(F<void *>(self, 0x4), dt);
    }

    TouchButton_setAlwaysPressed(*(void **)((char *)F<void *>(F<void *>(self, 0x0c), 0x4) + 4), true);
    ScrollTouchWindow_update(F<void *>(self, 0x2c), dt);

    if (F<uint8_t>(self, 0xa0) == 0) {
        float speed = F<float>(self, 0x94);
        float old = F<float>(self, 0x98);
        float delta = speed * old;
        float mag = -(speed * old);
        if (0.0f < delta) {
            mag = delta;
        }
        F<float>(self, 0x98) = delta;
        if (mag > 1.0f) {
            F<int>(self, 0x84) = (int)(delta + (float)F<int>(self, 0x84));
        }
    }

    int scroll = F<int>(self, 0x84);
    if (scroll > 0) {
        F<float>(self, 0x94) = 1.0f;
        F<float>(self, 0x98) = (float)-scroll * 0.5f;
    }

    int limit = F<int>(self, 0xa8) - F<int>(self, 0xa4);
    if (limit <= -1) {
        if (scroll < limit) {
            F<float>(self, 0x94) = 1.0f;
            F<float>(self, 0x98) = (float)(limit - scroll) * 0.5f;
        }
    } else {
        F<int>(self, 0x84) = 0;
        F<int>(self, 0x98) = 0;
    }
}
