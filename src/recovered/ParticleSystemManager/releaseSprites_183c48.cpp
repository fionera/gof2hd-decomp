#include "class.h"

extern "C" void _psm_ArrayReleaseSprites(void *arr);
extern "C" void _psm_ReleaseSpriteSystemResource(void *canvas, unsigned res);

// ParticleSystemManager::releaseSprites()
struct ParticleSystemManager { void releaseSprites(); };
void ParticleSystemManager::releaseSprites()
{
    _psm_ArrayReleaseSprites((char *)this + 0x18);
    if (u32(this, 0x30) != 0xffffffff) {
        _psm_ReleaseSpriteSystemResource(pp(this, 0x4), u32(this, 0x30));
        u32(this, 0x30) = 0xffffffff;
    }
}
