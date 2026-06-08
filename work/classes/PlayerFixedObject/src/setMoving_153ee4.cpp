#include "class.h"

extern "C" void PlayerFixedObject_setMoving(PlayerFixedObject *self, bool v)
{
    self->f_134 = v;
}
