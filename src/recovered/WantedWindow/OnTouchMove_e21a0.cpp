#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_WantedWindow_move_layout;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_move_screen_h;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_move_screen_w_a;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_move_force;
__attribute__((visibility("hidden"))) extern int *g_WantedWindow_move_screen_w_b;

extern "C" void StarMap_OnTouchMove(void *self, int x, int y);
extern "C" void ScrollTouchWindow_OnTouchMove(void *self, int x, int y);
extern "C" void TouchButton_OnTouchMove(void *self, int x, int y);
extern "C" void Layout_OnTouchMove(void *self, int x, int y);
extern "C" int Wanted_isActive(void *self);
extern "C" uint32_t WantedWindow_getWantedAtPosition(WantedWindow *self, int x, int y);

extern "C" int WantedWindow_OnTouchMove(WantedWindow *self, int x, int y)
{
    if (F<uint8_t>(self, 0x14) != 0) {
        StarMap_OnTouchMove(F<void *>(self, 0x4), x, y);
        return 0;
    }

    void **layoutHolder = g_WantedWindow_move_layout;
    void *layout = *layoutHolder;
    if (((F<int>(layout, 0x0c) < y) &&
         (y < *g_WantedWindow_move_screen_h - F<int>(layout, 0x10)) &&
         (x < *g_WantedWindow_move_screen_w_a / 2)) ||
        (*g_WantedWindow_move_force != 0)) {
        int delta = y - F<int>(self, 0x88);
        F<int>(self, 0x90) = delta;
        F<float>(self, 0x94) = 1.0f;
        F<int>(self, 0x84) += delta;
        F<int>(self, 0x88) = y;
    }

    if (*g_WantedWindow_move_screen_w_b / 2 < x) {
        ScrollTouchWindow_OnTouchMove(F<void *>(self, 0x2c), x, y);
    }

    uint32_t i = 0;
    while (i < F<uint32_t>(F<void *>(self, 0x0c), 0x0)) {
        TouchButton_OnTouchMove(ArrayItem(F<void *>(self, 0x0c), i), x, y);
        ++i;
    }

    Layout_OnTouchMove(*layoutHolder, x, y);
    if (F<void *>(self, 0x18) != 0) {
        void *wanted = ArrayItem(F<void *>(self, 0x38), F<int>(self, 0x30));
        if (Wanted_isActive(wanted) != 0) {
            TouchButton_OnTouchMove(F<void *>(self, 0x18), x, y);
        }
    }

    uint32_t selected = WantedWindow_getWantedAtPosition(self, x, y);
    int moved = y - F<int>(self, 0x9c);
    if (moved < 0) {
        moved = -moved;
    }
    if (moved > 5) {
        selected = 0xffffffffu;
    }
    F<uint32_t>(self, 0x34) = selected;
    return 0;
}
