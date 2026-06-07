#include "class.h"

void IParticleSystem::setParticleSetIndex(uint8_t index)
{
    U8(this, 0x44) = index;
}
