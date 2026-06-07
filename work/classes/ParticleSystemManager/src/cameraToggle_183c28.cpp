#include "class.h"

extern "C" void _psm_releaseSprites(void *self);
// re-init after camera change (PLT veneer); takes this.
extern "C" void _psm_constructAfterCamera(void *self);

// ParticleSystemManager::cameraToggle(ParticleSettings::CameraSet)
struct ParticleSystemManager { void cameraToggle(int cam); };
void ParticleSystemManager::cameraToggle(int cam)
{
    if (i32(this, 0xc) == cam)
        return;
    i32(this, 0xc) = cam;
    _psm_releaseSprites(this);
    return _psm_constructAfterCamera(this);
}
