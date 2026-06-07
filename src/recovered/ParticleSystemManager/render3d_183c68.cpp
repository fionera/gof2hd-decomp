#include "class.h"

extern "C" void _psm_renderMeshes(void *self);
extern "C" void _psm_renderSpritesExt(void *self);

// ParticleSystemManager::render3d()
struct ParticleSystemManager { void render3d(); };
void ParticleSystemManager::render3d()
{
    if (u8(this, 0x14) == 0)
        return;
    if (u8(this, 0x1) != 0)
        _psm_renderMeshes(this);
    if (u8(this, 0x0) == 0)
        return;
    return _psm_renderSpritesExt(this);
}
