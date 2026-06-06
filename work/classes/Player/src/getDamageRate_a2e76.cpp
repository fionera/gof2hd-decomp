#include "class.h"

extern "C" int Player_getDamageRate(Player *self)
{
    return self->damageRate;
}
