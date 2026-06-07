#include "class.h"

typedef void (*UpdateParticleFn)(IParticleSystem *, int, float);

void IParticleSystem::update(int delta)
{
    if (U8(this, 0xe) != 0) {
        float fdelta = (float)delta;
        for (int i = 0; i < I(this, 0x48); ++i) {
            if (((int *)P(this, 0x68))[i] != -1) {
                UpdateParticleFn fn = *(UpdateParticleFn *)((char *)P(this, 0x0) + 0x14);
                fn(this, i, fdelta);
            }
        }
    }
}
