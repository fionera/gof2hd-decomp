#include "class.h"

// 4-arg renderer (DAT_001ac974 veneer): canvas, a, b, c
extern "C" void _psm_meshRender4(void *canvas, unsigned a, unsigned b, unsigned c);
// 2-arg renderer (DAT_001ac984 veneer): canvas, a
extern "C" void _psm_meshRender2(void *canvas, unsigned a);

// ParticleSystemManager::renderMeshes()
struct ParticleSystemManager { void renderMeshes(); };
void ParticleSystemManager::renderMeshes()
{
    if (i16(this, 0x48) != -1)
        return _psm_meshRender2(pp(this, 0x4), u32(this, 0x58));
    if (i16(this, 0x26) != -1)
        return _psm_meshRender4(pp(this, 0x4), u32(this, 0x58), u32(this, 0x50), u32(this, 0x4c));
}
