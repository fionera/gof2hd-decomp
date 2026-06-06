#include "class.h"

extern "C" int Player_getMaxShieldHP(Player *self)
{
    return self->maxShieldHP;
}
