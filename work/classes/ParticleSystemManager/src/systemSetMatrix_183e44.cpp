#include "class.h"

// ParticleSystemManager::systemSetMatrix(int handle, AEMath::Matrix const* matrix)
// Bit-17 sprite/mesh dispatch; forwards the world matrix to the selected IParticleSystem.

extern "C" void _ips_setMatrix(void *sys, const void *matrix);

struct ParticleSystemManager { void systemSetMatrix(int handle, const void *matrix); };
void ParticleSystemManager::systemSetMatrix(int handle, const void *matrix)
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
    _ips_setMatrix(arr[idx], matrix);
}
