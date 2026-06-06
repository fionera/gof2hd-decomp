#include "class.h"

extern "C" int Player_getShieldDamageRate(Player *self)
{
    return self->shieldDamageRate;
}
