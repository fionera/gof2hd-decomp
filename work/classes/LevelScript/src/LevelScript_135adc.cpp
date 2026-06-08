#include "class.h"

extern "C" void Matrix_ctor(void *matrix);
extern "C" void Hud_drawTitleImage(Hud *hud, bool visible);
extern "C" int Level_getTimeLimit(Level *level);
extern "C" void *Level_getPlayer(Level *level);
extern "C" void Player_setVulnerable(void *player, bool vulnerable);
extern "C" void PlayerEgo_setCollide(void *player, bool collide);
extern "C" void TargetFollowCamera_setLookAtCam(TargetFollowCamera *camera, bool enabled);
extern "C" int Status_getCurrentCampaignMission(void *status);
extern "C" void *gStatus;

LevelScript::LevelScript(Level *level, Hud *hud, Radar *radar, TargetFollowCamera *camera)
{
    this->f_48 = 0;
    I(this, 0x38) = 0;
    I(this, 0x3c) = 0;
    I(this, 0x40) = 0;
    I(this, 0x44) = 0;
    I(this, 0x28) = 0;
    I(this, 0x2c) = 0;
    I(this, 0x30) = 0;
    I(this, 0x34) = 0;

    Matrix_ctor(B(this, 0x4c));

    this->f_d0 = hud;
    this->f_d4 = radar;
    this->m_pCamera = camera;
    this->m_pLevel = level;
    Hud_drawTitleImage(hud, false);

    I(this, 0x1c) = 0;
    US(this, 0x20) = 1;
    US(this, 0x10) = 0x100;

    I(this, 0x8c) = 0;
    I(this, 0x90) = 0;
    I(this, 0x94) = 0;
    I(this, 0x98) = 0;
    this->m_pGeometriesMission2 = 0;
    this->m_pExplosion = 0;
    this->m_pMission = 0;
    this->m_pPlayerEgo = 0;
    this->m_pAsteroids = 0;
    this->m_pExtra = 0;
    this->m_nParticleSystem0 = 0;
    this->m_nParticleSystem1 = 0;
    this->f_cc = 0;
    I(this, 0x8) = 0;
    I(this, 0xc) = 0;
    I(this, 0x0) = Level_getTimeLimit(level);
    I(this, 0x24) = 0;
    this->f_d8 = 0;
    this->f_dc = 0;
    US(this, 0x12) = 0;

    TargetFollowCamera_setLookAtCam(camera, true);

    void *player = Level_getPlayer(level);
    if (player == 0) {
        UC(this, 0x20) = 0;
    } else {
        Player_setVulnerable(player->f_0, false);
    }

    player = Level_getPlayer(level);
    PlayerEgo_setCollide(player, false);

    if (Status_getCurrentCampaignMission(gStatus) == 0) {
        I(this, 0x90) = 0;
        I(this, 0x94) = 0;
        UC(this, 0x21) = 1;
        TargetFollowCamera_setLookAtCam(camera, true);
    }
}
