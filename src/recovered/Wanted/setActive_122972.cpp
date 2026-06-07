#include "class.h"

extern "C" void Wanted_setActive(Wanted *self, bool v) {
    F<uint8_t>(self, 0x51) = v;
}
