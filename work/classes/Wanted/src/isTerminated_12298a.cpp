#include "class.h"

extern "C" uint8_t Wanted_isTerminated(Wanted *self) {
    return F<uint8_t>(self, 0x50);
}
