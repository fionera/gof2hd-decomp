#include "class.h"

extern "C" void TouchButton_setPosition(TouchButton *self, int x, int y, unsigned char flags);

extern "C" void TouchButton_setPosition2(TouchButton *self, int x, int y)
{
    return TouchButton_setPosition(self, x, y, UC(self, 0x74));
}
