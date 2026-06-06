#include "class.h"

extern "C" int Player_getArmorDamageRate(Player *self)
{
    return self->armorDamageRate;
}
