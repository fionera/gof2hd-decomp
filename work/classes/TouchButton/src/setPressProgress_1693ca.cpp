#include "class.h"

extern "C" void TouchButton_setPressProgress(TouchButton *self, float value)
{
    F(self, 0xbc) = value;
}
