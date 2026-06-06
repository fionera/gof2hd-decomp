#include "class.h"

extern "C" int Player_getMaxHitpoints(Player *self)
{
    return self->maxHitpoints;
}
