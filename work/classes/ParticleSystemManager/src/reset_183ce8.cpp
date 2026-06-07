#include "class.h"

// ParticleSystemManager::reset()
struct ParticleSystemManager { void reset(); };
void ParticleSystemManager::reset()
{
    for (unsigned i = 0; i < u32(this, 0x18); i++) {
        int *p = ((int **)u32(this, 0x1c))[i];
        if (p != 0)
            (*(void (**)(int *))(*p + 8))(p);
    }
    for (unsigned i = 0; i < u32(this, 0x3c); i++) {
        int *p = ((int **)u32(this, 0x40))[i];
        if (p != 0)
            (*(void (**)(int *))(*p + 8))(p);
    }
}
