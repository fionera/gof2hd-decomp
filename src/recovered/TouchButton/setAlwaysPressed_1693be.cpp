#include "class.h"

extern "C" void TouchButton_setAlwaysPressed(TouchButton *self, bool value)
{
    UC(self, 0xb1) = value;
}
