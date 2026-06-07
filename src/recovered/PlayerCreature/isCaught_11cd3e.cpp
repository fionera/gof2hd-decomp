#include "class.h"

struct PlayerCreature {
    uint8_t isCaught();
};

uint8_t PlayerCreature::isCaught()
{
    return F<uint8_t>(this, 0x12a);
}
