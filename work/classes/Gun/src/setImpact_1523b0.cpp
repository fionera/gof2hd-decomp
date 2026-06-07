#include "class.h"

struct Sparks;

extern "C" void Gun_setImpact(Gun *self, Sparks *impact)
{
    F<Sparks *>(self, 0xb8) = impact;
}
