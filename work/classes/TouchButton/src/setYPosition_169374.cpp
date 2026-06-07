#include "class.h"

extern "C" void TouchButton_setPosition(TouchButton *self, int x, int y, unsigned char flags);

extern "C" void TouchButton_setYPosition(TouchButton *self, int y)
{
    return TouchButton_setPosition(self, I(self, 0x78), y, UC(self, 0x74));
}
