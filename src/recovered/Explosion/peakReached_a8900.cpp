#include "class.h"

extern "C" bool Explosion_peakReached(Explosion *self)
{
    return *(long long *)B(self, 0x18) > 0x8fcLL;
}
