#include "class.h"

void IParticleSystem::enableEmit(bool enabled)
{
    if (enabled && U8(this, 0xc) == 0) {
        I(this, 0x60) = 0;
    }
    U8(this, 0xc) = enabled;
}
