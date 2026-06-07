#include "class.h"

extern "C" void _ips_update(void *sys, int dt);
extern "C" void _ips_resetEmitterVelocity(void *sys);
extern "C" void _ips_calcEmitterVelocity(void *sys, int accum);

// ParticleSystemManager::update(long long)
struct ParticleSystemManager { void update(long long dt); };
void ParticleSystemManager::update(long long dt)
{
    int d = (int)dt;
    if (u8(this, 0x14) == 0)
        return;
    int accum = i32(this, 0x10) + d;
    i32(this, 0x10) = accum;
    for (unsigned i = 0; i < u32(this, 0x18); i++) {
        void *p = ((void **)u32(this, 0x1c))[i];
        if (p != 0) {
            _ips_update(p, d);
            p = ((void **)u32(this, 0x1c))[i];
            if (u8(p, 0x4) == 0) {
                if (accum > 9 || u8(p, 0x5) != 0) {
                    _ips_calcEmitterVelocity(p, i32(this, 0x10));
                    p = ((void **)u32(this, 0x1c))[i];
                }
                (*(void (**)(void *, int))(*(int *)p + 4))(p, d);
            } else {
                _ips_resetEmitterVelocity(p);
            }
        }
    }
    for (unsigned i = 0; i < u32(this, 0x3c); i++) {
        void *p = ((void **)u32(this, 0x40))[i];
        if (p != 0) {
            _ips_update(p, d);
            p = ((void **)u32(this, 0x40))[i];
            if (u8(p, 0x4) == 0) {
                if (accum > 9 || u8(p, 0x5) != 0) {
                    _ips_calcEmitterVelocity(p, i32(this, 0x10));
                    p = ((void **)u32(this, 0x40))[i];
                }
                (*(void (**)(void *, int))(*(int *)p + 4))(p, d);
            } else {
                _ips_resetEmitterVelocity(p);
            }
        }
    }
    if (accum > 9)
        i32(this, 0x10) = 0;
}
