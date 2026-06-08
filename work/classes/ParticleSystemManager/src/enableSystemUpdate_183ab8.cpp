#include "class.h"

// ParticleSystemManager::enableSystemUpdate(int handle, bool enable)
// Handles pack the array selector in bit 17: when set, the system lives in the mesh array
// (+0x40) and the low bits index it; otherwise it lives in the sprite array (+0x1c). -1 means
// "no system". The selected IParticleSystem's update-enable setter is then invoked.

extern "C" void _ips_enableUpdate(void *sys, bool enable);

struct ParticleSystemManager { void enableSystemUpdate(int handle, bool enable); };
void ParticleSystemManager::enableSystemUpdate(int handle, bool enable)
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
    _ips_enableUpdate(arr[idx], enable);
}
