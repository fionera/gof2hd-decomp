#include "class.h"

extern "C" void TouchButton_setPressProgressHighlight(TouchButton *self, bool value)
{
    UC(self, 0xb8) = value;
}
