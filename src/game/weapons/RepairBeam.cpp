#include "game/weapons/RepairBeam.h"
#include "game/core/Globals.h"
#include "engine/render/AEGeometry.h"
#include "engine/render/PaintCanvas.h"
#include "game/ship/Ship.h"
#include "game/mission/Item.h"
#include "game/mission/Status.h"
#include "game/world/Level.h"
#include "game/ship/KIPlayer.h"
#include "engine/audio/FModSound.h"

namespace AbyssEngine {
    namespace AEMath {
        float VectorLength(const Vector &v);

        Vector VectorNormalize(const Vector &v);
    }
}

static PaintCanvas **g_RepairBeam_canvas;
static PaintCanvas **g_RB_canvas;
static int *g_RB_dmgThresh;
static float g_RB_scaleDiv;
static float g_RB_healMul;
static float g_RB_shieldMul;
static FModSound **g_RB_sndPlay;
static int *g_RB_sndPlayEv;
static FModSound **g_RB_sndStop;
static FModSound **g_RB_sndDead;
static FModSound **g_RB_sndUpd;
static int *g_RB_sndUpdEv;
static char **g_RB_sndFlag;


void RepairBeam::render() {
    Array<int> &ids = *this->targetIds;
    for (unsigned i = 0; i < ids.size(); ++i) {
        if (ids[i] != -1)
            (*this->geometries)[i]->render();
    }
}

RepairBeam::RepairBeam(int shipIndex, int sort) {
    this->sort = sort;
    this->shipIndex = shipIndex;
    this->beamPosition.x = 0;
    this->beamPosition.y = 0;
    this->beamPosition.z = 0;

    Ship *ship = Globals::status->getShip();
    Item *equip = ship->getFirstEquipmentOfSort(sort);
    int count = equip->getAttribute(/*RepairBeamCount*/ 0x37);

    this->geometries = new Array<AEGeometry *>();
    ArraySetLength<AEGeometry *>(count, *this->geometries);

    uint16_t geoId = (sort == 0x25) ? 0x4a94 : 0x4a95;
    PaintCanvas *canvas = *g_RepairBeam_canvas;
    for (int i = 0; i < count; ++i)
        (*this->geometries)[i] = new AEGeometry(geoId, canvas, false);

    this->targetIds = new Array<int>();
    ArraySetLength(count, *(this->targetIds));
    this->timer = 0x9c4;
    for (unsigned k = 0; k < this->targetIds->size(); ++k)
        (*this->targetIds)[k] = -1;

    this->charges = new Array<float>();
    ArraySetLength<float>(count, *this->charges);
    for (unsigned j = 0; j < this->charges->size(); ++j)
        (*this->charges)[j] = 0.0f;
}

RepairBeam::~RepairBeam() {
    if (this->geometries != nullptr) {
        ArrayReleaseClasses(*this->geometries);
        delete this->geometries;
        this->geometries = nullptr;
    }
    delete this->targetIds;
    this->targetIds = nullptr;
    delete this->charges;
    this->charges = nullptr;
}

void RepairBeam::update(int dt, Radar *radar, Level *level, Hud *hud) {
    (void) radar;
    (void) hud;

    Array<KIPlayer *> *enemies = level->getEnemies();
    this->timer -= dt;

    if (enemies != nullptr) {
        PlayerEgo *ego = level->getPlayer();
        if (ego->isDead() == 0) {
            Ship *ship = Globals::status->getShip();
            Item *equip = ship->getFirstEquipmentOfSort(this->sort);
            float attrF = (float) equip->getAttribute(0x35);

            Array<int> &ids = *this->targetIds;
            Vector &beamPos = this->beamPosition;

            if (this->timer < 0) {
                for (unsigned i = 0; i < ids.size(); ++i) {
                    ids[i] = -1;
                    (*this->charges)[i] = 0.0f;
                }
                this->timer += 0x9c4;

                for (unsigned e = 0; e < enemies->size(); ++e) {
                    KIPlayer *kp = (*enemies)[e];
                    if (kp->isDead() != 0 || kp->isDying() != 0)
                        continue;

                    bool consider = false;
                    if (this->sort == 0x25) {
                        Player *pl = kp->player;
                        if (pl->carriesFriendCargoFlag != 0 && kp->player->getHitpoints() < kp->player->getMaxHitpoints())
                            consider = true;
                    } else if (this->sort == 0x29) {
                        if (reinterpret_cast<uint8_t &>(kp->route) == 0 &&
                            kp->player->enemyFlagsLo != 0) {
                            Player **plp = (Player **) level->getPlayer();
                            if ((*plp)->getShieldDamageRate() < 100 &&
                                Globals::status->getShip()->getFirstEquipmentOfSort(9) != 0)
                                consider = true;
                        }
                    }
                    if (!consider)
                        continue;

                    Vector tmp;
                    tmp = kp->player->getPosition();
                    beamPos = tmp;
                    tmp = level->getPlayer()->getPosition();
                    beamPos -= tmp;
                    if (AbyssEngine::AEMath::VectorLength(beamPos) > attrF)
                        continue;

                    bool placed = false;
                    for (unsigned s = 0; !placed && s < ids.size(); ++s) {
                        if (ids[s] == -1) {
                            ids[s] = (int) e;
                            placed = true;
                        }
                    }
                    if (!placed) {
                        int srcHp = kp->player->getHitpoints();
                        unsigned best = 0xffffffffu;
                        int bestHp = *g_RB_dmgThresh;
                        for (unsigned s = 0; s < ids.size(); ++s) {
                            if (ids[s] != -1) {
                                int hp = (*enemies)[ids[s]]->player->getHitpoints();
                                if (hp < bestHp && srcHp < hp) {
                                    bestHp = hp;
                                    best = s;
                                }
                            }
                        }
                        if (best != 0xffffffffu)
                            ids[best] = (int) e;
                    }
                }
            }

            float scaleDiv = g_RB_scaleDiv;
            float dtF = (float) dt;
            PaintCanvas *canvas = *g_RB_canvas;
            float healAmt = dtF * g_RB_healMul;
            float shieldAmt = dtF * g_RB_shieldMul;
            bool allInactive = true;

            for (unsigned i = 0; i < ids.size(); ++i) {
                if (ids[i] == -1)
                    continue;

                AEGeometry *geo = (*this->geometries)[i];
                AbyssEngine::Transform *tr = (AbyssEngine::Transform *) canvas->TransformGetTransform(geo->transform);
                tr->Update(dt, false);

                Vector tmp;
                KIPlayer *enemy = (*enemies)[ids[i]];
                tmp = enemy->player->getPosition();
                beamPos = tmp;

                int kind = enemy->shipGroupFlag;

                Vector contrib;
                if (kind == 0x2c) {
                    Vector dir = AbyssEngine::AEMath::VectorNormalize(enemy->parentGeometry->getDirection());
                    dir *= -300.0f;
                    contrib = dir;
                } else if (kind == 0x31) {
                    Vector dir = AbyssEngine::AEMath::VectorNormalize(enemy->parentGeometry->getDirection());
                    dir *= -300.0f;
                    Vector up = AbyssEngine::AEMath::VectorNormalize(enemy->parentGeometry->getUpVector());
                    up *= 200.0f;
                    dir += up;
                    contrib = dir;
                } else {
                    contrib.x = 0;
                    contrib.y = 0;
                    contrib.z = 0;
                }
                beamPos += contrib;
                tmp = level->getPlayer()->getPosition();
                beamPos -= tmp;

                float len = AbyssEngine::AEMath::VectorLength(beamPos);
                geo->setScaling(len, 0.5f, 0.5f);

                Vector ndir = AbyssEngine::AEMath::VectorNormalize(beamPos);
                Vector beamUp;
                beamUp.x = 0.0f;
                beamUp.y = 1.0f;
                beamUp.z = 0.0f;
                geo->setDirection(ndir, beamUp);

                tmp = level->getPlayer()->getPosition();
                geo->setPosition(tmp);

                AbyssEngine::Transform *tr2 = (AbyssEngine::Transform *) canvas->TransformGetTransform(geo->transform);
                tr2->Update(dt, false);

                if (this->sort == 0x29) {
                    Player **plp = (Player **) level->getPlayer();
                    if ((*plp)->getShieldDamageRate() < 100) {
                        Item *eq = Globals::status->getShip()->getFirstEquipmentOfSort(this->sort);
                        float a = (float) eq->getAttribute(0x36);
                        float &charge = (*this->charges)[i];
                        charge += (shieldAmt * a) / scaleDiv;
                        if (charge < 1.0f) {
                            enemy->player->damage(1, false, -1);
                            charge -= 1.0f;
                        }
                        Player **plp2 = (Player **) level->getPlayer();
                        Item *eq2 = Globals::status->getShip()->getFirstEquipmentOfSort(this->sort);
                        float a2 = (float) eq2->getAttribute(0x36);
                        (*plp2)->regenerateShield((shieldAmt * a2) / scaleDiv);
                    }
                } else if (this->sort == 0x25) {
                    Item *eq = Globals::status->getShip()->getFirstEquipmentOfSort(this->sort);
                    float a = (float) eq->getAttribute(0x36);
                    enemy->player->heal((healAmt * a) / scaleDiv);
                }

                if ((*g_RB_sndPlay)->isPlaying(g_RB_sndPlayEv[this->shipIndex]) == 0)
                    (*g_RB_sndPlay)->play(g_RB_sndPlayEv[this->shipIndex], nullptr, nullptr, 0.0f);
                allInactive = false;
            }
            if (allInactive)
                (*g_RB_sndStop)->stop(g_RB_sndPlayEv[this->shipIndex]);
        }
    }

    PlayerEgo *ego2 = level->getPlayer();
    if (ego2->isDead() != 0) {
        if ((*g_RB_sndDead)->isPlaying(g_RB_sndPlayEv[this->shipIndex]) != 0)
            (*g_RB_sndDead)->stop(g_RB_sndPlayEv[this->shipIndex]);
    }

    if ((*g_RB_sndFlag)[0xf] != 0) {
        FModSound *snd = *g_RB_sndUpd;
        if (snd->isPlaying(g_RB_sndUpdEv[this->shipIndex]) != 0) {
            Vector tmp;
            tmp = level->getPlayer()->getPosition();
            this->beamPosition = tmp;
            snd->updateEvent3DAttributes(g_RB_sndUpdEv[this->shipIndex], &this->beamPosition, nullptr, false);
        }
    }
}
