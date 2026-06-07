#include "class.h"

// 4-arg sprite renderer (DAT_001ac994 veneer): canvas, a, b, c
extern "C" void _psm_spriteRender4(void *canvas, unsigned a, unsigned b, unsigned c);
// 2-arg sprite renderer (DAT_001ac9a4 veneer): canvas, a
extern "C" void _psm_spriteRender2(void *canvas, unsigned a);

// ParticleSystemManager::renderSprites()
struct ParticleSystemManager { void renderSprites(); };
void ParticleSystemManager::renderSprites()
{
    if (i16(this, 0x24) != -1)
        return _psm_spriteRender2(pp(this, 0x4), u32(this, 0x30));
    if (i16(this, 0x26) != -1)
        return _psm_spriteRender4(pp(this, 0x4), u32(this, 0x30), u32(this, 0x2c), u32(this, 0x28));
}
