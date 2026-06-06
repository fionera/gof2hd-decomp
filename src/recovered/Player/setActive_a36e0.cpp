#include "class.h"

extern "C" void Player_setActive(Player *self, bool value)
{
    self->active = value;
}
