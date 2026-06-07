#include "class.h"

struct RadioBlock16 {
    uint64_t a;
    uint64_t b;
};

struct RadioPair {
    uint32_t a;
    uint32_t b;
};

extern "C" __attribute__((visibility("hidden"))) void **g_Radio_layout;
extern "C" __attribute__((visibility("hidden"))) int **g_Radio_screenWidth;

Radio::Radio()
{
    void **layoutHolder = g_Radio_layout;

    F<RadioPair>(this, 0x18) = RadioPair{0, 0};
    F<uint32_t>(this, 0x28) = 0;
    F<uint16_t>(this, 0x2c) = 0;
    F<uint32_t>(this, 0x10) = 0;
    F<int>(this, 0x30) = -1;
    F<RadioBlock16>(this, 0x00) = RadioBlock16{0, 0};

    void *layout = *layoutHolder;
    int x = F<int>(layout, 0x98);
    int screenWidth = **g_Radio_screenWidth;
    F<int>(this, 0x38) = x;
    F<int>(this, 0x3c) = (screenWidth - x) >> 1;
    F<int>(this, 0x40) = F<int>(layout, 0x9c);
}
