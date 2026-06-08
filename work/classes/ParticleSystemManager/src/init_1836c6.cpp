#include "class.h"

// ParticleSystemManager::init()
// Builds the sprite and mesh sub-systems, marks the manager active (+0x14), and runs the first
// update tick (via the virtual update at vtable+? -> here the resolved emitter-update entry).

extern "C" void _psm_initSprites(void *self);
extern "C" void _psm_initMesh(void *self);
extern "C" int  _psm_firstUpdate(void *self, int a, int b, int c);

struct ParticleSystemManager { int init(); };
int ParticleSystemManager::init()
{
    _psm_initSprites(this);
    _psm_initMesh(this);
    u8(this, 0x14) = 1;
    return _psm_firstUpdate(this, 0, 0, 0);
}
