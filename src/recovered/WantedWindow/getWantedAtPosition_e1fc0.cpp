#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_WantedWindow_hit_layout;
extern "C" int __aeabi_idiv(int numerator, int denominator);

extern "C" uint32_t WantedWindow_getWantedAtPosition(WantedWindow *self, int x, int y)
{
    if (x >= F<int>(self, 0x1c) + (F<int>(self, 0x24) >> 1)) {
        return 0xffffffffu;
    }

    void *list = F<void *>(self, 0x38);
    void *layout = *g_WantedWindow_hit_layout;
    int numerator = y - F<int>(self, 0x20);
    numerator -= F<int>(layout, 0x0c);
    numerator -= F<int>(layout, 0x20);
    numerator -= F<int>(layout, 0x5c);
    numerator -= F<int>(self, 0x84);
    int idx = __aeabi_idiv(numerator, F<int>(layout, 0x70) + F<int>(layout, 0x34));
    if ((uint32_t)idx > (uint32_t)(F<int>(list, 0x0) - 1)) {
        return 0xffffffffu;
    }
    return (uint32_t)idx;
}
