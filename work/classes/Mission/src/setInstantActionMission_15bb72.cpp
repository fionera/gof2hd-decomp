#include "class.h"

extern "C" void Mission_setInstantActionMission(Mission *self, bool v) {
    F<uint8_t>(self, 0x5c) = v;
}
