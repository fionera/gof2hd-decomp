#include "class.h"

extern "C" void Gun_setMagnitude(Gun *self, int v)
{
    self->f_100 = v;
}
