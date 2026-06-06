#include "class.h"

extern "C" void Player_setVulnerable(Player *self, bool value)
{
    self->vulnerable = value;
}
