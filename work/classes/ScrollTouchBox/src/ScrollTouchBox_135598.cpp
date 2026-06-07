#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_ScrollTouchBox_defaultFont_135598;

struct Int4 {
    int a;
    int b;
    int c;
    int d;
};

ScrollTouchBox::ScrollTouchBox(int x, int y, int width, int height)
{
    F<int>(this, 0x28) = 0;
    F<void *>(this, 0x0) = 0;
    F<int>(this, 0x4) = x;
    F<int>(this, 0x8) = y;
    F<int>(this, 0xc) = width;
    F<int>(this, 0x10) = height;
    F<int>(this, 0x14) = width;
    F<uint8_t>(this, 0x30) = 0;
    F<int>(this, 0x34) = 0;
    F<uint8_t>(this, 0x38) = 0;
    F<Int4>(this, 0x18) = Int4();
    F<void *>(this, 0x3c) = *g_ScrollTouchBox_defaultFont_135598;
}
