#include "class.h"

__attribute__((visibility("hidden"))) extern int **g_NewsTicker_touchBegin_font;
__attribute__((visibility("hidden"))) extern int *g_NewsTicker_touchBegin_screen;

uint8_t NewsTicker::OnTouchBegin(int x, int y)
{
    if (this->f_4 <= x &&
        x <= this->f_4 + this->f_c &&
        this->f_8 <= y) {
        int fontHeight = (*g_NewsTicker_touchBegin_font)[4];
        int bottom = *g_NewsTicker_touchBegin_screen;
        bottom += 2;
        __builtin_sub_overflow(bottom, fontHeight, &bottom);
        if (y <= bottom) {
            *(volatile int *)((char *)this + 0x2c) = x;
            *(volatile uint8_t *)((char *)this + 0x28) = 1;
        }
    }
    return this->f_28;
}
