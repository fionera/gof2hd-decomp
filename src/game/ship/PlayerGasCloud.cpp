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
    this->cloudMeshId = (unsigned int) (itemId - 201) < 4 ? itemId + 18796 : 18997;
    this->itemId = itemId;
    this->sparkMeshId = (unsigned int) (itemId - 201) < 4 ? itemId + 18800 : 19001;
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
        Vector target = {x, y, z};
        Vector direction = VectorNormalize(this->center - target);

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

    if (!this->exploded || this->settled || this->sparkGeometries == nullptr) {
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->modelGeometry->transform))
                ->Update(dt, true);
        return;
    }

    float time = (float) dt;
    this->elapsedSinceExplosion += dt;
    this->settled = 1;
    float decay = time * 0.08f;
    for (unsigned int i = 0; i < this->sparkGeometries->size(); i++) {
        (*this->sparkTimers)[i] -= dt;
        (*this->sparkLife)[i] -= decay;
        if ((*this->sparkLife)[i] < (*this->sparkLifeMin)[i])
            (*this->sparkLife)[i] = (*this->sparkLifeMin)[i];

        Vector turret = this->level->getPlayer()->getTurretPosition();
        Vector spark = (*this->sparkGeometries)[i]->getPosition();
        float distance = VectorLength(turret - spark);
        if (distance < 800.0f && this->elapsedSinceExplosion >= 2000 &&
            this->level->getPlayer()->isInTurretMode() && (*this->sparkTimers)[i] > -1500) {
            // Collected by the ego's turret.
            (*this->sparkTimers)[i] = -1500;
            if (Globals::status->getShip()->getFreeSpace() > 0) {
                Item *item = (*Globals::items)[this->itemId]->makeItem(1);
                if (this->level->getPlayer() != nullptr) {
                    ((Hud *) (intptr_t) this->level->getPlayer()->getHUD())
                            ->catchCargo(this->itemId, 1, false, false, false, false, true);
                    if (!Globals::hints[45] && Globals::status->getCurrentCampaignMission() >= 143 &&
                        Globals::status->getMission()->isEmpty() && this->itemId == 204) {
                        Globals::hints[45] = 1;
                        this->level->createRadioMessage(26, 0);
                    }
                }
                Globals::sound->stop(2256);
                Globals::sound->play(2256, nullptr, nullptr, 0.0f);
                Globals::status->getShip()->addCargo(item);
            } else if (this->level->getPlayer() != nullptr) {
                Globals::sound->stop(2256);
                Globals::sound->play(2256, nullptr, nullptr, 0.0f);
                ((Hud *) (intptr_t) this->level->getPlayer()->getHUD())
                        ->catchCargo(this->itemId, 0, true, false, true, false, false);
            }
            (*this->sparkScale)[i] = 0.0f;
        } else {
            if ((*this->sparkTimers)[i] <= 0) {
                (*this->sparkScale)[i] = (float) (*this->sparkTimers)[i] / 1500.0f + 1.0f;
                if ((*this->sparkScale)[i] < 0.0f)
                    (*this->sparkScale)[i] = 0.0f;
            } else if (distance < 3500.0f) {
                (*this->sparkScale)[i] = (distance + -800.0f) / 2700.0f;
            }
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform((*this->sparkGeometries)[i]->transform))
                    ->Update(dt, true);
        }

        if ((*this->sparkInSight)[i] && this->elapsedSinceExplosion >= 2000 &&
            Globals::status->getShip()->getFirstEquipmentOfSort(35) != nullptr) {
            // A gas collector pulls the sparks towards the turret.
            Vector direction = VectorNormalize(turret - spark);
            *(*this->sparkVelocities)[i] = direction;
            (*this->sparkGeometries)[i]->setPosition(
                    spark + (float) (Globals::status->getShip()->getFirstEquipmentOfSort(35)->getAttribute(49) * dt) *
                            direction);
        } else {
            (*this->sparkGeometries)[i]->setPosition(spark + (*this->sparkLife)[i] * time * *(*this->sparkVelocities)[i]);
        }
        if ((*this->sparkTimers)[i] > -1500)
            this->settled = 0;
    }
    if (this->settled)
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

