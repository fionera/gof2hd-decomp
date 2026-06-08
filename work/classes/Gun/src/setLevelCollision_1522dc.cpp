#include "class.h"

extern "C" void Gun_setLevelCollision(Gun *self, bool v)
{
    self->f_f8 = v;
}
