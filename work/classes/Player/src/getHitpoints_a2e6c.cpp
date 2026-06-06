#include "class.h"

extern "C" int Player_getHitpoints(Player *self)
{
    return self->hitpoints;
}
