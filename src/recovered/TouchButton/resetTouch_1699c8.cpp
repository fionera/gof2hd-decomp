#include "class.h"

extern "C" void TouchButton_resetTouch(TouchButton *self)
{
    UC(self, 0xb0) = 0;
}
