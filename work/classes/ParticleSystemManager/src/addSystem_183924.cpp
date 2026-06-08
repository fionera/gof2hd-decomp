#include "class.h"

// ParticleSystemManager::addSystem(AEMath::Matrix const*, ParticleSettings::ParticleSet, bool)
// The 16-byte target lifts as bad ARM data (a tail-call veneer). It is a thin forwarder that
// constructs a particle system for the given set and registers it; we model it as a tail-call to
// the sprite-system add path (the default add route), returning its handle.

extern "C" int _psm_addSpriteSystem(void *self, const void *matrix, unsigned int set, bool flag);

struct ParticleSystemManager {
    int addSystem(const void *matrix, unsigned int set, bool flag);
};
int ParticleSystemManager::addSystem(const void *matrix, unsigned int set, bool flag)
{
    return _psm_addSpriteSystem(this, matrix, set, flag);
}
