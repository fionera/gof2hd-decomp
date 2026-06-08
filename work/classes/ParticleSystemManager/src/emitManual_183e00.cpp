#include "class.h"

// ParticleSystemManager::emitManual(int handle, Vector const& pos, int ret, float)
// 4-argument overload: bit-17 sprite/mesh dispatch, emits one burst at `pos`. Returns the
// pass-through `ret` selector packed into the low word (the high word is -1 on the no-system
// path, matching the original's CONCAT44).

extern "C" void _ips_emitManual(void *sys, float x, float y, float z);

struct ParticleSystemManager {
    unsigned long long emitManual(int handle, const float *pos, int ret, float p4);
};
unsigned long long ParticleSystemManager::emitManual(int handle, const float *pos, int ret, float p4)
{
    (void)p4;
    if (handle == -1)
        return ((unsigned long long)0xffffffffu << 32) | (unsigned int)(unsigned long)this;

    void **arr;
    int idx;
    if (handle << 0x11 < 0) {
        arr = (void **)u32(this, 0x40);
        idx = handle & 0x3fffffff;
    } else {
        arr = (void **)u32(this, 0x1c);
        idx = handle;
    }
    _ips_emitManual(arr[idx], pos[0], pos[1], pos[2]);
    return (unsigned int)ret;
}
