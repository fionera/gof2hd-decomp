#include "class.h"

extern "C" unsigned char Player_doesNeverAttack(Player *self)
{
    return self->neverAttack;
}
