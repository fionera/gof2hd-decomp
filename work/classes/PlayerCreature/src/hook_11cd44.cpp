#include "class.h"

extern "C" void PlayerCreature_hook_tail(PlayerCreature *self, int value);

struct PlayerCreature {
    void hook(int);
};

void PlayerCreature::hook(int value)
{
    F<uint8_t>(this, 0x129) = 1;
    return PlayerCreature_hook_tail(this, value);
}
