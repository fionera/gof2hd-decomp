#include "class.h"

extern "C" unsigned char Player_isDamaged(Player *self)
{
    return self->damaged;
}
