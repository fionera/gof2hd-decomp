#include "class.h"

extern "C" void TouchButton_setVisible(TouchButton *self, bool value)
{
    UC(self, 0xb2) = value;
}
