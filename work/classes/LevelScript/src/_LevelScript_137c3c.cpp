#include "class.h"

extern "C" void *AEGeometry_dtor(void *geometry);
extern "C" void *Explosion_dtor(void *explosion);
extern "C" void operator_delete(void *ptr);

LevelScript::~LevelScript()
{
    void *geometry = this->f_dc;
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    this->f_dc = 0;

    geometry = this->f_d8;
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    this->f_d8 = 0;

    geometry = this->m_pPlayerEgo;
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    this->m_pPlayerEgo = 0;

    geometry = this->m_pAsteroids;
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    this->m_pAsteroids = 0;

    geometry = this->m_pExtra;
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    this->m_pExtra = 0;

    geometry = this->m_nParticleSystem0;
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    this->m_nParticleSystem0 = 0;

    geometry = this->m_pGeometriesMission2;
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    this->m_pGeometriesMission2 = 0;

    geometry = this->m_pExplosion;
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    this->m_pExplosion = 0;

    geometry = this->m_pMission;
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    this->m_pMission = 0;

    void *explosion = this->m_nParticleSystem1;
    if (explosion != 0) {
        operator_delete(Explosion_dtor(explosion));
    }
    this->m_nParticleSystem1 = 0;

    geometry = this->f_cc;
    if (geometry != 0) {
        operator_delete(AEGeometry_dtor(geometry));
    }
    this->f_cc = 0;
}
