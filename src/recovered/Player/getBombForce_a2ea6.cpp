#include "class.h"

extern "C" float Player_getBombForce(Player *self)
{
    return self->bombForce;
}
