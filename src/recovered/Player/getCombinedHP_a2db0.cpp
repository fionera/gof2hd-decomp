#include "class.h"

extern "C" int Player_getCombinedHP(Player *self)
{
    return (int)(self->shieldHP + (float)self->armorHP + (float)self->hitpoints);
}
