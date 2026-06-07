#include "class.h"

extern "C" uint8_t Mission_isInstantActionMission(Mission *self) {
    return F<uint8_t>(self, 0x5c);
}
