#include "class.h"

typedef void (*RenderProc)(void *);

extern "C" void AEGeometry_render(void *geometry);
extern "C" int Explosion_isPlaying(void *explosion);
extern "C" void Explosion_render(void *explosion);

static volatile RenderProc gRenderProc;

void LevelScript::render3D()
{
    if (this->f_dc != 0) {
        AEGeometry_render(this->f_dc);
    }
    if (this->f_d8 != 0) {
        AEGeometry_render(this->f_d8);
    }
    if (this->m_pPlayerEgo != 0) {
        AEGeometry_render(this->m_pPlayerEgo);
    }
    if (this->m_pAsteroids != 0) {
        AEGeometry_render(this->m_pAsteroids);
    }
    if (this->m_pExtra != 0) {
        AEGeometry_render(this->m_pExtra);
    }
    if (this->m_nParticleSystem0 != 0) {
        AEGeometry_render(this->m_nParticleSystem0);
    }
    if (this->m_nParticleSystem1 != 0 && Explosion_isPlaying(this->m_nParticleSystem1) != 0) {
        Explosion_render(this->m_nParticleSystem1);
    }
    if (UC(this, 0xa8) != 0 && this->m_pGeometriesMission2 != 0) {
        RenderProc render = gRenderProc;
        render(this->m_pGeometriesMission2);
        render(this->m_pExplosion);
        return render(this->m_pMission);
    }
}
