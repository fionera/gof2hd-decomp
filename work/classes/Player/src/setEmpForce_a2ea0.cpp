#include "class.h"

extern "C" void Player_setEmpForce(Player *self, float value)
{
    self->empForce = value;
}
