#include "class.h"

// ParticleSystemManager::enableSystemRender(int handle, bool enable)
// Bad-data ARM veneer that mirrors the other per-system setters' bit-17 sprite/mesh dispatch,
// forwarding to the IParticleSystem render-enable method.

extern "C" void _ips_enableRender(void *sys, bool enable);

struct ParticleSystemManager { void enableSystemRender(int handle, bool enable); };
void ParticleSystemManager::enableSystemRender(int handle, bool enable)
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
    _ips_enableRender(arr[idx], enable);
}
