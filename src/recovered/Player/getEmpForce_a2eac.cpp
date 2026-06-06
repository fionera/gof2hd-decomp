#include "class.h"

extern "C" float Player_getEmpForce(Player *self)
{
    return self->empForce;
}
