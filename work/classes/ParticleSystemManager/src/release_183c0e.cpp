#include "class.h"

extern "C" void _psm_releaseSprites(void *self);
// Array<ParticleSystemMesh*>::release-ish, via PLT veneer; takes &mesh-array (this+0x3c).
extern "C" void _psm_releaseMeshArray(void *arr);

// ParticleSystemManager::release()
struct ParticleSystemManager { void release(); };
void ParticleSystemManager::release()
{
    _psm_releaseSprites(this);
    u32(this, 0x4) = 0;
    return _psm_releaseMeshArray((char *)this + 0x3c);
}
