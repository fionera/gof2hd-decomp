#include "class.h"

__attribute__((visibility("hidden"))) extern int *g_NewsTicker_touchMove_screenWidth;

bool NewsTicker::OnTouchMove(int x, int)
{
    uint8_t touched = this->f_28;
    if (touched != 0) {
        float delta = (float)(this->f_2c - x);
        float current = this->f_0;
        int *limit = g_NewsTicker_touchMove_screenWidth;
        float next = current - delta;
        this->f_0 = next;
        float maxX = (float)*limit;
        if (maxX < next) {
            this->f_0 = maxX;
        }
        this->f_2c = x;
    }
    return touched != 0;
}
