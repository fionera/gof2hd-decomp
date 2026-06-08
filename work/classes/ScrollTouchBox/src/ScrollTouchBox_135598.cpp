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
    this->f_28 = 0;
    this->f_0 = 0;
    this->f_4 = x;
    this->f_8 = y;
    this->f_c = width;
    this->f_10 = height;
    this->f_14 = width;
    this->f_30 = 0;
    this->f_34 = 0;
    this->f_38 = 0;
    this->f_18 = Int4();
    this->f_3c = *g_ScrollTouchBox_defaultFont_135598;
}
