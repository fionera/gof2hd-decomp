#include "class.h"

extern "C" void BluePrint_unlock(BluePrint *self)
{
    F<uint8_t>(self, 0x8) = 1;
}
