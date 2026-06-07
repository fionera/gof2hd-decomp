#include "class.h"

extern "C" void Wanted_setTerminated(Wanted *self, bool v) {
    F<uint8_t>(self, 0x50) = v;
}
