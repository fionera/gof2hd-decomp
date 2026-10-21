#include "game/ship/PlayerGasCloud.h"
#include "game/core/Globals.h"

#include <cstddef>

#include "engine/core/AERandom.h"
#include "engine/render/AEGeometry.h"
#include "engine/audio/FModSound.h"
#include "engine/math/Transform.h"
#include "game/mission/Item.h"
#include "game/mission/Status.h"
#include "game/mission/Mission.h"
#include "game/ui/Hud.h"
#include "game/world/Level.h"
#include "game/ship/Ship.h"
#include "game/ship/Player.h"
#include "game/ship/PlayerEgo.h"
#include "engine/render/PaintCanvas.h"
#include "game/world/Station.h"

namespace AbyssEngine {
    namespace AEMath {
        float VectorLength(const Vector &value);
    }
}

struct PgcCampaign {
    char reserved0[0x2d];
    char rescueMissionFlag;
};
#if __SIZEOF_POINTER__ == 4
static_assert(offsetof(PgcCampaign, rescueMissionFlag) == 0x2d,
              "PgcCampaign.rescueMissionFlag must be at 0x2d");
#endif





static float g_pgcu_velScale = 0.08f;
static float g_pgcu_catchDist = 800.0f;
static int g_pgcu_minTimer = -1499;
static int g_pgcu_resetTimer = -1500;
static float g_pgcu_fadeLo = 3500.0f;
static float g_pgcu_fadeAdd = -800.0f;
static float g_pgcu_fadeDiv = 2700.0f;
static float g_pgcu_growDiv = 1500.0f;

PlayerGasCloud::PlayerGasCloud(int itemId, ParticleSystemManager * /*particles*/,
                               AEGeometry *geometry, const Vector &position)
    : KIPlayer(itemId, -1, new Player(0, 9999999, 0, 0, 0), geometry,
               position.x, position.y, position.z, false) {
    this->player->setKIPlayer(this);
    this->player->setMaxHitpoints(1);
    this->setPosition(position);

    this->crateGeometry = 0;
    this->elapsedSinceExplosion = 0;
    this->field_0x25 = 0;
    // Gas cloud items 201..204 have their own cloud/spark meshes.
    int cloudMesh = 18997;
    if ((unsigned int) (itemId - 201) < 4)
        cloudMesh = itemId + 18796;
    int sparkMesh = 19001;
    if ((unsigned int) (itemId - 201) < 4)
        sparkMesh = itemId + 18800;
    this->cloudMeshId = cloudMesh;
    this->itemId = itemId;
    this->sparkMeshId = sparkMesh;
    this->sparkGeometries = 0;
    this->sparkVelocities = 0;
    this->sparkLife = 0;
    this->sparkLifeMin = 0;
    this->sparkScale = 0;
    this->sparkTimers = 0;
    this->sparkInSight = 0;
    this->exploded = 0;
    this->center = position;

    this->modelGeometry = new AEGeometry((uint16_t) this->cloudMeshId, Globals::Canvas, false);
    this->modelGeometry->setPosition(position);
    this->field_0x44 = 1;
    this->hasCargo = 1;
    this->state = 0;
    this->visibleFlag = 1;
    this->settled = 0;
}

PlayerGasCloud::~PlayerGasCloud() {
    if (this->sparkGeometries != 0) {
        ArrayReleaseClasses(*this->sparkGeometries);
        delete this->sparkGeometries;
        this->sparkGeometries = 0;
    }

    if (this->sparkVelocities != 0) {
        ArrayReleaseClasses(*this->sparkVelocities);
        delete this->sparkVelocities;
        this->sparkVelocities = 0;
    }

    delete this->sparkLife;
    this->sparkLife = 0;

    delete this->sparkLifeMin;
    this->sparkLifeMin = 0;

    delete this->sparkScale;
    this->sparkScale = 0;

    delete this->sparkTimers;
    this->sparkTimers = 0;

    delete this->modelGeometry;
    this->modelGeometry = 0;
}

void PlayerGasCloud::translate(const Vector &v) {
    this->geometry->translate(v);
}

bool PlayerGasCloud::isSparkAlive(int index) {
    Array<AEGeometry *> *arr = this->sparkGeometries;
    if (arr == 0)
        return false;
    if (arr->size() <= (unsigned int) index)
        return false;
    return -1500 < (*this->sparkTimers)[index];
}

void PlayerGasCloud::setSparkInSight(int index, bool inSight) {
    Array<AEGeometry *> *arr = this->sparkGeometries;
    if (arr == 0)
        return;
    if ((unsigned int) index < arr->size())
        (*this->sparkInSight)[index] = inSight;
}

void PlayerGasCloud::setPosition(const Vector &position) {
    this->geometry->setPosition(position);
}

void *PlayerGasCloud::getSparks() { // lint: void_ptr (exported method return type baked into ABI)
    // lint: void_ptr (exported method return type baked into ABI)
    return this->sparkGeometries;
}

Vector PlayerGasCloud::getPosition() {
    return this->geometry->getPosition();
}

uint8_t PlayerGasCloud::hasExploded() {
    return this->exploded;
}

void PlayerGasCloud::explode(int itemIndex, Vector src, float radius) {
    if (this->exploded != 0)
        return;

    this->state = 3;
    this->setActive(false);
    this->exploded = 1;
    this->sparkGeometries = new Array<AEGeometry *>();
    this->sparkVelocities = new Array<Vector *>();
    this->sparkLife = new Array<float>();
    this->sparkLifeMin = new Array<float>();
    this->sparkTimers = new Array<int>();
    this->sparkInSight = new Array<bool>();
    this->sparkScale = new Array<float>();

    // The closer the explosion, the more (and further spread) sparks the cloud releases.
    Vector delta = src - this->center;
    float strength = 1.5f - VectorLength(delta) / radius;
    float amount = strength * 130.0f;
    float density = (float) (*Globals::items)[itemIndex]->getAttribute(56) / 100.0f;
    float spread = strength * 5000.0f;
    int count = (int) ((amount / 1.5f + 10.0f) * density);
    for (int i = 0; i < count; i++) {
        AEGeometry *spark = new AEGeometry((uint16_t) this->sparkMeshId, Globals::Canvas, false);
        spark->setPosition(this->center);

        float x = this->center.x + delta.x - spread + strength * (float) Globals::rnd->nextInt(10000);
        float y = this->center.y + delta.y - spread + strength * (float) Globals::rnd->nextInt(10000);
        float z = this->center.z - spread + delta.z + strength * (float) Globals::rnd->nextInt(10000);
        Vector direction = VectorNormalize(Vector{x, y, z} - this->center);

        float minLife = (float) Globals::rnd->nextInt(200) / 200.0f * 3.0f + 3.0f;
        float life = minLife * 7.0f;
        int timer = Globals::rnd->nextInt(14000);
        ArrayAdd(life, *this->sparkLife);
        ArrayAdd(minLife, *this->sparkLifeMin);
        ArrayAdd(timer + 8000, *this->sparkTimers);
        ArrayAdd(new Vector(direction), *this->sparkVelocities);
        ArrayAdd(spark, *this->sparkGeometries);
        ArrayAdd(false, *this->sparkInSight);
        ArrayAdd(1.0f, *this->sparkScale);
    }
}

void PlayerGasCloud::update(int dt) {
    if (dt == 0)
        return;

    Array<AEGeometry *> *arr = this->sparkGeometries;
    if (this->exploded == 0 || this->settled != 0 || arr == 0) {
        AbyssEngine::Transform *t = (AbyssEngine::Transform *)
                (Globals::Canvas)->TransformGetTransform(
                    this->modelGeometry->transform);
        t->Update(1, (bool) dt);
        return;
    }

    float dtf = (float) dt;
    this->settled = 1;
    this->elapsedSinceExplosion += dt;
    float velStep = dtf * g_pgcu_velScale;

    unsigned int i = 0;
    while (i < arr->size()) {
        int *timer = &(*this->sparkTimers)[i];
        *timer -= dt;

        float *life = &(*this->sparkLife)[i];
        float newLife = *life - velStep;
        *life = newLife;
        float lifeMin = (*this->sparkLifeMin)[i];
        if (newLife < lifeMin)
            *life = lifeMin;

        PlayerEgo *player = (PlayerEgo *) (intptr_t) this->level->getPlayer();
        Vector turretPos = player->getTurretPosition();
        Vector shardPos;
        Vector delta;
        AEGeometry *geom = (*this->sparkGeometries)[i];
        shardPos = geom->getPosition();
        delta = turretPos - shardPos;
        float dist = VectorLength(delta);

        bool collected = false;
        if (dist < g_pgcu_catchDist && this->elapsedSinceExplosion >= 2000) {
            PlayerEgo *p2 = (PlayerEgo *) (intptr_t) this->level->getPlayer();
            if (p2->isInTurretMode() != 0 &&
                (*this->sparkTimers)[i] >= g_pgcu_minTimer) {
                (*this->sparkTimers)[i] = g_pgcu_resetTimer;
                Ship *ship = Globals::status->getShip();
                int itemId = this->itemId;
                if (ship->getFreeSpace() < 1) {
                    if (this->level->getPlayer() != 0) {
                        Globals::sound->stop(0x8d0);
                        Globals::sound->play(0x8d0, 0, 0, 0.0f);
                        PlayerEgo *ego = (PlayerEgo *) (intptr_t) this->level->getPlayer();
                        Hud *hud = (Hud *) (intptr_t) ego->getHUD();
                        hud->catchCargo(this->itemId, false, true, false, true, false, 0);
                    }
                } else {
                    Item *def = (*(Globals::items))[itemId];
                    def->makeItem();
                    if (this->level->getPlayer() != 0) {
                        PlayerEgo *ego = (PlayerEgo *) (intptr_t) this->level->getPlayer();
                        Hud *hud = (Hud *) (intptr_t) ego->getHUD();
                        hud->catchCargo(this->itemId, true, false, false, false, false, 0);

                        PgcCampaign *camp = (PgcCampaign *) Globals::hints;
                        if (camp->rescueMissionFlag == 0 &&
                            Globals::status->getCurrentCampaignMission() > 0x8e) {
                            Mission *mission = Globals::status->getMission();
                            if (mission->isEmpty() != 0 && this->itemId == 0xcc) {
                                camp->rescueMissionFlag = 1;
                                this->level->createRadioMessage(0x1a, 0);
                            }
                        }
                    }
                    Globals::sound->stop(0x8d0);
                    Globals::sound->play(0x8d0, 0, 0, 0.0f);
                    Ship *ship2 = Globals::status->getShip();
                    ship2->addCargo(def);
                }
                (*this->sparkScale)[i] = 0.0f;
                collected = true;
            }
        }

        if (!collected) {
            int tval = (*this->sparkTimers)[i];
            float *scale = &(*this->sparkScale)[i];
            if (tval <= 0) {
                float sc = (float) tval / g_pgcu_growDiv + 1.0f;
                *scale = sc;
                if (sc < 0.0f)
                    *scale = 0.0f;
            } else if (dist < g_pgcu_fadeLo) {
                *scale = (dist + g_pgcu_fadeAdd) / g_pgcu_fadeDiv;
            }
            AbyssEngine::Transform *t = (AbyssEngine::Transform *)
                    (Globals::Canvas)->TransformGetTransform(
                        geom->transform);
            t->Update(1, (bool) dt);
        }

        AEGeometry *moveGeom;
        Vector moved;
        bool homing = (*this->sparkInSight)[i] != 0 &&
                      this->elapsedSinceExplosion >= 2000;
        bool steered = false;
        if (homing) {
            Ship *ship = Globals::status->getShip();
            if (ship->getFirstEquipmentOfSort(0x23) != 0) {
                Vector dir = turretPos - shardPos;
                Vector dn = AbyssEngine::AEMath::VectorNormalize(dir);
                *(*this->sparkVelocities)[i] = dn;
                moveGeom = (*this->sparkGeometries)[i];

                Ship *ship2 = Globals::status->getShip();
                Item *eq = ship2->getFirstEquipmentOfSort(0x23);
                int attr = eq->getAttribute(0);
                float step = (float) (attr * dt);
                Vector vel = turretPos;
                vel *= step;
                vel += shardPos;
                moved = vel;
                moveGeom->setPosition(moved);
                steered = true;
            }
        }
        if (!steered) {
            moveGeom = (*this->sparkGeometries)[i];
            Vector vel = *(*this->sparkVelocities)[i];
            vel *= (*this->sparkLife)[i] * dtf;
            vel += shardPos;
            moved = vel;
            moveGeom->setPosition(moved);
        }

        if (g_pgcu_minTimer <= (*this->sparkTimers)[i])
            this->settled = 0;
        i++;
        arr = this->sparkGeometries;
    }

    if (this->settled != 0)
        this->visibleFlag = 0;
}

void PlayerGasCloud::render() {
    if (this->visibleFlag == 0)
        return;
    if (this->state != 3 && this->state != 0)
        return;

    // Billboard the cloud towards the camera.
    Matrix camera = *(const Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent());
    Vector direction = -MatrixGetDir(camera);
    if (this->exploded && this->sparkGeometries != nullptr) {
        for (unsigned int i = 0; i < this->sparkGeometries->size(); i++) {
            (*this->sparkGeometries)[i]->setScaling((*this->sparkScale)[i]);
            (*this->sparkGeometries)[i]->setDirection(direction, MatrixGetUp(camera));
            (*this->sparkGeometries)[i]->render();
        }
    } else {
        this->modelGeometry->setDirection(direction, MatrixGetUp(camera));
        this->modelGeometry->render();
    }
}

