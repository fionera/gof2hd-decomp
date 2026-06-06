#include "class.h"

extern "C" int Player_getShieldHP(Player *self)
{
    return (int)self->shieldHP;
}
