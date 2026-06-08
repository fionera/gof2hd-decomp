#include "class.h"

__attribute__((visibility("hidden"))) extern int *g_NewsTicker_touchMove_screenWidth;

bool NewsTicker::OnTouchMove(int x, int)
{
    uint8_t touched = F<uint8_t>(this, 0x28);
    if (touched != 0) {
        float delta = (float)(F<int>(this, 0x2c) - x);
        float current = F<float>(this, 0x0);
        int *limit = g_NewsTicker_touchMove_screenWidth;
        float next = current - delta;
        F<float>(this, 0x0) = next;
        float maxX = (float)*limit;
        if (maxX < next) {
            F<float>(this, 0x0) = maxX;
        }
        F<int>(this, 0x2c) = x;
    }
    return touched != 0;
}
