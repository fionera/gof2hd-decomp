#include "class.h"

extern "C" uint8_t TouchButton_isVisible(TouchButton *self)
{
    return UC(self, 0xb2);
}
