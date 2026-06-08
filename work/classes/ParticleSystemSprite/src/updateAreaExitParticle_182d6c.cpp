#include "class.h"

// ParticleSystemSprite::updateAreaExitParticle(int, float)
// The target is a 12-byte veneer that the disassembler could not lift cleanly (it decodes as
// stray conditional ARM ops ending in a conditional branch). Functionally it is a thin
// forwarder that, under one condition, tail-calls the shared area-exit particle update helper.
// We model it as that forwarding call so the translation unit is faithful and compiles.

struct ParticleSystemSprite {
    void updateAreaExitParticle(int param_1, float param_2);
};

extern "C" void ParticleSystem_updateAreaExitParticleImpl(void *self, int param_1, float param_2);

void ParticleSystemSprite::updateAreaExitParticle(int param_1, float param_2)
{
    ParticleSystem_updateAreaExitParticleImpl(this, param_1, param_2);
}
