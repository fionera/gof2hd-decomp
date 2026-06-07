#include "class.h"

extern "C" void TB_assignString(void *dst, String *src);

extern "C" void TouchButton_setNumberText(TouchButton *self, String *value)
{
    return TB_assignString((char *)self + 0x2c, value);
}
