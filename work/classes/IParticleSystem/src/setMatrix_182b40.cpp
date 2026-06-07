#include "class.h"

void IParticleSystem::setMatrix(Matrix const *matrix)
{
    *(Matrix const * volatile *)((char *)this + 0x18) = matrix;
    *(volatile uint16_t *)((char *)this + 0x4) = 0x100;
}
