#include "class.h"

extern "C" void Player_setNeverAttack(Player *self, bool value)
{
    self->neverAttack = value;
}
