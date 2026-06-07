#include "class.h"

extern "C" void BluePrint_lock(BluePrint *self)
{
    F<uint8_t>(self, 0x8) = 1;
}
