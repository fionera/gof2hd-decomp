#include "class.h"

// Complete object destructor (D1): set vtable, run release(), tail-call base dtor.
extern "C" char ParticleSystemSprite_vtable;
extern "C" void _ZN20ParticleSystemSprite7releaseEv(void *self); // release()
extern "C" void *ParticleSystemSprite_baseDtor(void *self);

extern "C" void *_ZN20ParticleSystemSpriteD1Ev(void *self)
{
    *(void **)self = &ParticleSystemSprite_vtable + 8;
    _ZN20ParticleSystemSprite7releaseEv(self);
    return ParticleSystemSprite_baseDtor(self);
}
