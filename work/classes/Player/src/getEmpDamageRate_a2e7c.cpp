#include "class.h"

extern "C" int Player_getEmpDamageRate(Player *self)
{
    return self->empDamageRate;
}
