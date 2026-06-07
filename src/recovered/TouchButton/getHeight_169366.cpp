#include "class.h"

extern "C" int TouchButton_getHeight(TouchButton *self)
{
    return I(self, 0x88);
}
