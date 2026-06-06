#include "class.h"

extern "C" void Player_setRadius(Player *self, int value)
{
    self->radius = value;
}
