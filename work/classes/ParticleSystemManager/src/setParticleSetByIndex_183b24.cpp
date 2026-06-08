#include "class.h"

// ParticleSystemManager::setParticleSetByIndex(int handle, unsigned char setIndex)
// Same bit-17 sprite/mesh dispatch as the other per-system setters; forwards the set index to
// the selected IParticleSystem.

extern "C" void _ips_setParticleSetByIndex(void *sys, unsigned char setIndex);

struct ParticleSystemManager { void setParticleSetByIndex(int handle, unsigned char setIndex); };
void ParticleSystemManager::setParticleSetByIndex(int handle, unsigned char setIndex)
{
    if (handle == -1)
        return;
    void **arr;
    int idx;
    if (handle << 0x11 < 0) {
        arr = (void **)u32(this, 0x40);
        idx = handle & 0x3fffffff;
    } else {
        arr = (void **)u32(this, 0x1c);
        idx = handle;
    }
    _ips_setParticleSetByIndex(arr[idx], setIndex);
}
