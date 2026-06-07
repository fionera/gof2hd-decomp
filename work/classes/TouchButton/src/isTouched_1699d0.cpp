#include "class.h"

extern "C" uint8_t TouchButton_isTouched(TouchButton *self)
{
    return UC(self, 0xb0);
}
