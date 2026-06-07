#include "class.h"

extern "C" void TouchButton_translate(TouchButton *self, int dx, int dy)
{
    I(self, 0x78) = dx + I(self, 0x78);
    I(self, 0x7c) = dy + I(self, 0x7c);
}
