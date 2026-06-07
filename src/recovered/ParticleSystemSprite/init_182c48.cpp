#include "class.h"

struct ParticleSystemSprite {
    int init(unsigned int param_1, unsigned short param_2);
};

int ParticleSystemSprite::init(unsigned int param_1, unsigned short param_2)
{
    void **vt = *(void ***)this;
    F<unsigned int>(this, 0x54) = param_1;
    F<unsigned int>(this, 0x58) = param_2;
    F<uint8_t>(this, 0x5c) = 1;
    typedef int (*pfn)(void *);
    return ((pfn)vt[2])(this);
}
