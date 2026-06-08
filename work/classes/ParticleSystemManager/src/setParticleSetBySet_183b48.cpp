#include "class.h"

// ParticleSystemManager::setParticleSetBySet(int handle, ParticleSettings::ParticleSet set)
// Bit-17 sprite/mesh dispatch; forwards a particle-set descriptor (passed by value as a single
// word in this build) to the selected IParticleSystem.

extern "C" void _ips_setParticleSetBySet(void *sys, unsigned int set);

struct ParticleSystemManager { void setParticleSetBySet(unsigned int handle, unsigned int set); };
void ParticleSystemManager::setParticleSetBySet(unsigned int handle, unsigned int set)
{
    if (handle == 0xffffffffu)
        return;
    void **arr;
    unsigned int idx;
    if ((int)(handle << 0x11) < 0) {
        arr = (void **)u32(this, 0x40);
        idx = handle & 0x3fffffff;
    } else {
        arr = (void **)u32(this, 0x1c);
        idx = handle;
    }
    _ips_setParticleSetBySet(arr[idx], set);
}
