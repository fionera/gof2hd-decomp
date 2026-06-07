#include "class.h"

extern "C" void TouchButton_setHalfTransparent(TouchButton *self, bool value)
{
    UC(self, 0xb3) = value;
}
