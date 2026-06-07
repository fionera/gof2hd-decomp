#include "class.h"

struct PlayerCreature {
    uint8_t isHooked();
};

uint8_t PlayerCreature::isHooked()
{
    return F<uint8_t>(this, 0x129);
}
