#include "class.h"

// ParticleSystemManager::addSpriteSystem(AEMath::Matrix const*, Array<ParticleSet> const&, bool)
// Allocates a ParticleSystemSprite (0x78 bytes), constructs it against the manager's canvas
// (+0x04) and the sprite flag (+0x38), appends it to the sprite array (+0x18), accumulates its
// particle count into +0x34, and returns the new system's sprite-array handle (index, no flag).

void *operator new(uint32_t size);

extern "C" void *_pss_ctor(void *self, void *canvas, const void *matrix, const void *sets,
                           bool b4, bool b5);                       // ParticleSystemSprite ctor
extern "C" void _psm_arraySpriteAdd(void *sys, void *arr);          // ArrayAdd<ParticleSystemSprite*>
extern "C" int  _ips_getParticleCount(void *sys);                  // IParticleSystem::getParticleCount

struct ParticleSystemManager {
    int addSpriteSystem(const void *matrix, const void *sets, bool flag);
};
int ParticleSystemManager::addSpriteSystem(const void *matrix, const void *sets, bool flag)
{
    void *sys = operator new(0x78);
    _pss_ctor(sys, pp(this, 0x04), matrix, sets, flag, u8(this, 0x38) != 0);
    _psm_arraySpriteAdd(sys, (char *)this + 0x18);
    i32(this, 0x34) = _ips_getParticleCount(sys) + i32(this, 0x34);
    return i32(this, 0x18) - 1;
}
