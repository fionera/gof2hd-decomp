#include "class.h"

extern "C" int TouchButton_getWidth(TouchButton *self)
{
    return I(self, 0x90);
}
