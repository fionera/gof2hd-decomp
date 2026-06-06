#include "class.h"

extern "C" int Player_getGammaHP(Player *self)
{
    return (int)self->gammaHP;
}
