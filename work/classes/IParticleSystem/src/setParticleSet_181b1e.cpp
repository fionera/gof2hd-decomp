#include "class.h"

void IParticleSystem::setParticleSet(int set)
{
    if (I(this, 0x38) != 0 && *(int *)F<void *>(this, 0x3c) == set) {
        U8(this, 0x44) = 0;
    }
}
