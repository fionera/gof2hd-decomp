#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_WantedWindow_touch_layout;

extern "C" void StarMap_OnTouchBegin(void *self, int x, int y);
extern "C" void ScrollTouchWindow_OnTouchBegin(void *self, int x, int y);
extern "C" void TouchButton_OnTouchBegin(void *self, int x, int y);
extern "C" void Layout_OnTouchBegin(void *self, int x, int y);
extern "C" int Wanted_isActive(void *self);
extern "C" uint32_t WantedWindow_getWantedAtPosition(WantedWindow *self, int x, int y);

extern "C" int WantedWindow_OnTouchBegin(WantedWindow *self, int x, int y)
{
    if (F<uint8_t>(self, 0x14) != 0) {
        StarMap_OnTouchBegin(F<void *>(self, 0x4), x, y);
        return 0;
    }

    F<int>(self, 0x88) = y;
    F<int>(self, 0x9c) = y;
    F<int>(self, 0x90) = 0;
    F<uint8_t>(self, 0xa0) = 1;
    ScrollTouchWindow_OnTouchBegin(F<void *>(self, 0x2c), x, y);

    uint32_t i = 0;
    while (i < F<uint32_t>(F<void *>(self, 0x0c), 0x0)) {
        TouchButton_OnTouchBegin(ArrayItem(F<void *>(self, 0x0c), i), x, y);
        ++i;
    }

    Layout_OnTouchBegin(*g_WantedWindow_touch_layout, x, y);
    if (F<void *>(self, 0x18) != 0) {
        void *wanted = ArrayItem(F<void *>(self, 0x38), F<int>(self, 0x30));
        if (Wanted_isActive(wanted) != 0) {
            TouchButton_OnTouchBegin(F<void *>(self, 0x18), x, y);
        }
    }

    F<uint32_t>(self, 0x34) = WantedWindow_getWantedAtPosition(self, x, y);
    return 0;
}
