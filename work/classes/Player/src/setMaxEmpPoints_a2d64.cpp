#include "class.h"

extern "C" void Player_setMaxEmpPoints_tail();

extern "C" void Player_setMaxEmpPoints(Player *self, int value)
{
    self->empPoints = value;
    self->maxEmpPoints = value;
    return Player_setMaxEmpPoints_tail();
}
