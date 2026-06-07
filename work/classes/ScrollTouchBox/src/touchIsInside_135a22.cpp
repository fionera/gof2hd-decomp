#include "class.h"

bool ScrollTouchBox::touchIsInside(int x, int y)
{
    char *base = (char *)this;
    int left = *(int *)(base + 0x4);
    int top;
    if (left <= x)
        top = *(int *)(base + 0x8);
    if (left > x || top > y)
        return false;

    bool inside = false;
    if (x < left + *(int *)(base + 0xc))
        inside = y < top + *(int *)(base + 0x10);
    return inside;
}
