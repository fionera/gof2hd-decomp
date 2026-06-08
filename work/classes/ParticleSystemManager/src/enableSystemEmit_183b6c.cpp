#include "class.h"

// ParticleSystemManager::enableSystemEmit(int handle, bool enable)
// The target lifts as bad ARM data (a tail-call veneer), but it is the same bit-17 sprite/mesh
// handle dispatch as the sibling setters, forwarding to the IParticleSystem emit-enable method.

extern "C" void _ips_enableEmit(void *sys, bool enable);

struct ParticleSystemManager { void enableSystemEmit(int handle, bool enable); };
void ParticleSystemManager::enableSystemEmit(int handle, bool enable)
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
    _ips_enableEmit(arr[idx], enable);
}
