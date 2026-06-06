#include "class.h"

extern "C" void Player_updateDamageRate(Player *self);

extern "C" void Player_setEmpData(Player *self, int points, int data)
{
    self->empPoints = points;
    if (self->maxEmpPoints < points) {
        self->maxEmpPoints = points;
    }
    Player_updateDamageRate(self);
    self->empData = data;
}
