#include "class.h"

extern "C" void Player_setShootingEnabled(Player *self, bool value)
{
    self->shootingEnabled = value;
}
