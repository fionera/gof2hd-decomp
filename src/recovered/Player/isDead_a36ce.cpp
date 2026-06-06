#include "class.h"

extern "C" bool Player_isDead(Player *self)
{
    return self->hitpoints < 1;
}
