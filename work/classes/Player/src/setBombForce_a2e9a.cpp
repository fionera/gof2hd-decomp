#include "class.h"

extern "C" void Player_setBombForce(Player *self, float value)
{
    self->bombForce = value;
}
