#include "class.h"

__attribute__((visibility("hidden"))) extern int **g_NewsTicker_getHeight_font;
__attribute__((visibility("hidden"))) extern int *g_NewsTicker_getHeight_screen;

int NewsTicker::getHeight()
{
    int fontHeight = (*g_NewsTicker_getHeight_font)[4];
    int top = *g_NewsTicker_getHeight_screen;
    top += 2;
    int bottom;
    __builtin_sub_overflow(top, fontHeight, &bottom);
    return bottom - F<int>(this, 0x8);
}
