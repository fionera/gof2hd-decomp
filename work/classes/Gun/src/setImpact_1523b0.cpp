#include "class.h"

struct Sparks;

extern "C" void Gun_setImpact(Gun *self, Sparks *impact)
{
    self->f_b8 = impact;
}
