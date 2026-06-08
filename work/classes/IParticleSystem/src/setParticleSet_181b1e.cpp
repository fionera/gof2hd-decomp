#include "class.h"

void IParticleSystem::setParticleSet(int set)
{
    if (I(this, 0x38) != 0 && *(int *)this->f_3c == set) {
        U8(this, 0x44) = 0;
    }
}
