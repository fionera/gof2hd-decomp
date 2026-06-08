#include "class.h"

extern "C" void Gun_setMagnitude(Gun *self, int v)
{
    F<int>(self, 0x100) = v;
}
