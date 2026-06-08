#include "class.h"

// ParticleSystemManager::resetSystem(int handle)
// Bad-data ARM veneer; same bit-17 sprite/mesh handle dispatch, tail-calling the selected
// IParticleSystem's reset method.

extern "C" void _ips_reset(void *sys);

struct ParticleSystemManager { void resetSystem(int handle); };
void ParticleSystemManager::resetSystem(int handle)
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
    _ips_reset(arr[idx]);
}
