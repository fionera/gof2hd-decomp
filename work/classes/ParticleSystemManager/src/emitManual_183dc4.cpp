#include "class.h"

// ParticleSystemManager::emitManual(int handle, Vector const& pos, int ret, Vector const& vel, float)
// Bit-17 sprite/mesh dispatch; emits one burst at the given position into the selected system.
// The decompiled return is the pass-through `ret` selector word when a system was hit, else
// `this`. Modeled as the dispatch + emit; we return the selector for fidelity.
//
// NB: the first explicit param is `this` (in r0); the engine passes the manager pointer there.

extern "C" void _ips_emitManual(void *sys, float x, float y, float z);

extern "C" void *ParticleSystemManager_emitManual_v(
    void *self, int handle, const float *pos, void *ret, const float *vel, float p5)
{
    (void)vel; (void)p5;
    void *result = self;
    if (handle != -1) {
        void **arr;
        int idx;
        if (handle << 0x11 < 0) {
            arr = (void **)u32(self, 0x40);
            idx = handle & 0x3fffffff;
        } else {
            arr = (void **)u32(self, 0x1c);
            idx = handle;
        }
        _ips_emitManual(arr[idx], pos[0], pos[1], pos[2]);
        result = ret;
    }
    return result;
}
