#include "class.h"

struct ParticleSystemSprite {
    void release();
};

extern "C" void operator_delete__(void *p);

void ParticleSystemSprite::release()
{
    if (F<void *>(this, 0x64) != 0)
        operator_delete__(F<void *>(this, 0x64));
    F<void *>(this, 0x64) = 0;
    if (F<void *>(this, 0x68) != 0)
        operator_delete__(F<void *>(this, 0x68));
    F<void *>(this, 0x68) = 0;
    operator_delete__(F<void *>(this, 0x6c));
    F<void *>(this, 0x6c) = 0;
}
