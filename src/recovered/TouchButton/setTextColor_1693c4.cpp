#include "class.h"

extern "C" void TouchButton_setTextColor(TouchButton *self, int color)
{
    I(self, 0xac) = color;
}
