#include "class.h"

extern "C" void Gun_setPlayerGun(Gun *self, bool v)
{
    self->f_f0 = v;
}
