#include "game/ship/PlayerFixedObject.h"
#include "engine/core/AERandom.h"
#include "game/ship/TargetFollowCamera.h"
#include "engine/render/AEGeometry.h"
#include "engine/audio/FModSound.h"
#include "game/mission/Generator.h"
#include "engine/render/LODManager.h"
#include "engine/render/ParticleSystemManager.h"
#include "engine/math/Transform.h"
#include "game/mission/Achievements.h"
#include "game/mission/Explosion.h"
#include "game/ui/Hud.h"
#include "game/ship/KIPlayer.h"
#include "game/world/Level.h"
#include "game/mission/Mission.h"
#include "game/ship/PlayerEgo.h"
#include "game/world/Standing.h"
#include "game/mission/Status.h"
#include "engine/math/BoundingVolume.h"
#include "game/ship/Player.h"
#include "engine/render/PaintCanvas.h"
#include "game/core/Globals.h"
#include "game/world/Station.h"

namespace AbyssEngine {
    namespace AEMath {
        float VectorLength(const Vector &value);
    }
}

// Pirate stations: station index -> loot list parameters.
static const int kPirateStationIds[4] = {1, 33, 47, 86};
static const int kPirateStationLoot[4][2] = {{45, 5}, {28, 1}, {4, 1}, {17, 1}};

PlayerFixedObject::PlayerFixedObject(int kind, int race, Player *player, AEGeometry *geom,
                                     float x, float y, float z)
    : KIPlayer(kind, -1, player, geom, x, y, z, false), respawnPos(), homingTarget(), homingDir(), field_0x15c(),
      field_0x160(), field_0x164() {
    this->wreckMeshId = 0;
    this->explosion = nullptr;
    this->wreckGeometry = nullptr;
    this->boundingVolumes = nullptr;
    this->faction() = race;
    this->wreckCollision = nullptr;
    this->deltaTime = 0;
    this->targetEnemy = 0;
    this->collisionIndex = 0;
    this->field_0x170 = 0;
    this->shipHidden = 0;
    this->explosionElapsed = 0;
    this->field_0x174 = 0;
    this->intPosX = 0;
    this->intPosY = 0;
    this->intPosZ = 0;
    this->position() = Vector{x, y, z};
    this->wreckType = -1;
    this->moving = 0;
    this->wreckMaterial = -1;
    this->dockingType = 0;
    this->intPosY = (int) y;
    this->intPosX = (int) x;
    this->intPosZ = (int) z;
    this->name = String("", false);

    if (Globals::status->getMission()->isCampaignMission()
        && (Globals::status->getCurrentCampaignMission() == 40 || Globals::status->getCurrentCampaignMission() == 41)) {
        delete this->lootList();
        this->lootList() = nullptr;
    } else {
        Generator *generator = new Generator();
        if (kind == 0x37a3) {
            this->field_0x41() = 1;
            int stationIndex = Globals::status->getStation()->getIndex();
            for (unsigned int i = 0; i < 4; i++) {
                if (kPirateStationIds[i] == stationIndex)
                    this->lootList() = generator->getLootList(kPirateStationLoot[i][0], kPirateStationLoot[i][1]);
            }
        } else {
            this->lootList() = generator->getLootList(-1, -1);
            if (this->lootList() && kind != 0x498e && kind != 0x4a88) {
                for (unsigned int i = 1; i - 1 < this->lootList()->size(); i += 2) {
                    if (kind == 14) {
                        (*this->lootList())[i] *= Globals::rnd->nextInt(8) + 5;
                    } else {
                        (*this->lootList())[i] *= Globals::rnd->nextInt(4) + 2;
                        int minimum = Globals::rnd->nextInt(5) + 8;
                        (*this->lootList())[i] = minimum > (*this->lootList())[i] ? minimum : (*this->lootList())[i];
                    }
                }
            }
        }
        delete generator;
    }

    this->player->spawnedFlag = 1;
    if (kind != 0x37a3) {
        this->aiActiveCounter() = 47;
        if (kind == 14) {
            this->moving = 0;
            this->aiActiveCounter() = -1;
        }
    }
}

PlayerFixedObject::~PlayerFixedObject() {
    if (this->wreckGeometry != this->geometry) {
        delete this->wreckGeometry;
        this->wreckGeometry = nullptr;
    }
    if (this->boundingVolumes) {
        ArrayReleaseClasses(*this->boundingVolumes);
        delete this->boundingVolumes;
    }
    this->boundingVolumes = nullptr;
    if (this->wreckCollision) {
        ArrayReleaseClasses(*this->wreckCollision);
        delete this->wreckCollision;
    }
    this->wreckCollision = nullptr;
    delete this->explosion;
    this->explosion = nullptr;
}

void PlayerFixedObject::reset() {
    this->KIPlayer::reset();

    this->setPosition(this->spawnX(), this->spawnY(), this->spawnZ());

    this->targetEnemy = 0;

    this->respawnPos = Vector{this->spawnX(), this->spawnY(), this->spawnZ()};
    this->position() = Vector(this->respawnPos);

    this->deltaTime = 0;
    if (this->state != 5)
        this->state = 0;

    this->targetPos() = Vector{0.0f, 0.0f, 0.0f};
    this->homingTarget = Vector{0.0f, 0.0f, 0.0f};
    this->homingDir = Vector{0.0f, 0.0f, 0.0f};
}

void PlayerFixedObject::setMoving(bool v) {
    this->moving = v;
}

void PlayerFixedObject::setPosition(float x, float y, float z) {
    this->spawnX() = x;
    this->spawnY() = y;
    this->spawnZ() = z;
    this->intPosY = (int32_t) y;
    this->intPosX = (int32_t) x;
    this->intPosZ = (int32_t) z;
    ((AEGeometry *) this->geometry)->setPosition(x, y, z);
    reinterpret_cast<Matrix &>(this->player->transform[0]) = ((AEGeometry *) this->geometry)->getMatrix();
    this->position() = ((AEGeometry *) this->geometry)->getPosition();
    if (this->boundingVolumes != nullptr) {
        for (uint32_t i = 0; i < this->boundingVolumes->size(); i++)
            (*this->boundingVolumes)[i]->update(this->position().x, this->position().y, this->position().z);
    }
    if (this->wreckGeometry != nullptr)
        this->wreckGeometry->setPosition(x, y, z);
}

void PlayerFixedObject::setPosition(const Vector &v) {
    this->setPosition(v.x, v.y, v.z);
}

void PlayerFixedObject::translate(const Vector &d) {
    float x = (float) this->intPosX;
    float y = (float) this->intPosY;
    float z = (float) this->intPosZ;
    this->setPosition(d.x + x, d.y + y, d.z + z);
}

void PlayerFixedObject::setBV(Array<BoundingVolume *> *bv) {
    this->boundingVolumes = bv;
}

void PlayerFixedObject::setBV(BoundingVolume *bv) {
    Array<BoundingVolume *> *a = new Array<BoundingVolume *>();
    this->boundingVolumes = a;
    ArrayAdd(bv, *a);
}

V3 PlayerFixedObject::getPosition() {
    float fy = (float) intPosY;
    float fx = (float) intPosX;
    float fz = (float) intPosZ;
    V3 pos = {fx, fy, fz};
    return pos;
}

void PlayerFixedObject::setWreckedMeshId(int meshId) {
    this->wreckMeshId = (uint16_t) meshId;
    this->wreckGeometry = new AEGeometry((uint16_t) meshId, Globals::Canvas, true);
    ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->wreckGeometry->transform))
        ->boundingSphere.radius = 500000.0f;
    switch (this->kind()) {
        case 13:
            this->wreckType = 4;
            break;
        case 14:
            this->wreckType = 0;
            break;
        case 15:
            if (this->faction() == 3)
                this->wreckType = 1;
            else if (this->faction() == 2)
                this->wreckType = 2;
            else
                this->wreckType = 3;
            break;
        case 0x37a3:
            this->wreckType = 5;
            break;
        default:
            if (this->wreckType < 0)
                return;
            break;
    }
    this->wreckCollision = Globals::globals->getWreckCollision(this->wreckType, this->wreckGeometry);
}

void PlayerFixedObject::moveForward(int amount) {
    float d = (float) amount;
    this->intPosZ = amount + this->intPosZ;
    ((AEGeometry *) (this->geometry))->moveForward(d);
    const Matrix *m = &((AEGeometry *) (this->geometry))->getMatrix();
    ((Player *) this->player)->transform = *m;
    Vector position = ((AEGeometry *) this->geometry)->getPosition();
    this->position() = position;
    if (this->wreckGeometry != 0) {
        this->wreckGeometry->moveForward(d);
    }
    Array<BoundingVolume *> *bv = this->boundingVolumes;
    if (bv != 0) {
        for (uint32_t i = 0; i < bv->size(); i++) {
            BoundingVolume *o = (*bv)[i];
            o->update(this->position().x, this->position().y, this->position().z);
            bv = this->boundingVolumes;
        }
    }
}

void PlayerFixedObject::update(int dt) {
    this->deltaTime = dt;
    this->player->update(dt, this->aiActiveCounter() != -1 && this->moving);

    this->player->enemyFlagsLo = (this->faction() & ~1) == 8 ? true
        : ((Standing *) (intptr_t) Globals::status->getStanding())->isEnemy(this->faction());
    this->player->carriesFriendCargoFlag =
        (this->faction() & ~1) == 8 ? false
                                    : ((Standing *) (intptr_t) Globals::status->getStanding())->isFriend(this->faction());
    if (this->player->turnedEnemy())
        reinterpret_cast<uint16_t &>(this->player->enemyFlagsLo) = 1;
    if (this->player->isAlwaysFriend())
        reinterpret_cast<uint16_t &>(this->player->enemyFlagsLo) = 0x100;

    if (this->state != 6) {
        float bombForce = this->player->getBombForce();
        float empForce = this->player->getEmpForce();
        if (bombForce > 0.0f) {
            bombForce *= 0.98f;
            if (bombForce < 0.05f)
                bombForce = 0.0f;
            this->player->setBombForce(bombForce);
        }
        if (empForce > 0.0f) {
            empForce -= (float) dt;
            this->empActive() = !(empForce < 0.05f);
            if (empForce < 0.05f)
                empForce = 0.0f;
            this->player->setEmpForce(empForce);
        }
    }

    if (!this->empActive() && (unsigned) (this->state - 3) >= 2) {
        if (this->kind() != 0x37a3 && this->moving)
            this->moveForward(dt);
        if (!(Globals::status->getCurrentCampaignMission() == 91 && this->kind() == 0x494e)
            && !(this->kind() == 0x494a && Globals::status->getCurrentCampaignMission() == 145)
            && this->kind() != 0x4220)
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->geometry->transform))->Update(dt, true);
    }

    if (this->player->getHitpoints() <= 0 && (unsigned) (this->state - 3) >= 2) {
        if (this->player->enemyFlagsLo)
            this->level->enemyDied(this->kind(), this->player->destroyedByte);
        else
            this->level->friendDied();
        if (this->kind() == 0x37a3)
            this->level->pirateStationAction(false);
        this->state = 3;
        this->moving = 0;
        this->hasCargo = this->cargoAvailable();
        if (this->hasCargo)
            this->createCrate(0);
        this->setExhaustVisible(false);
        this->wreckGeometry->setMatrix(this->geometry->getMatrix());

        if (this->wreckGeometry) {
            this->wreckGeometry->setMatrix(this->geometry->getMatrix());
            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->wreckGeometry->transform))
                ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
            if (this->faction() == 3 && this->moving && this->geometry->parentTransform != (uint32_t) -1)
                this->wreckGeometry->addChild(this->geometry->parentTransform);
            this->level->field_74->systemSetMatrix(
                this->kind() == 0x37a3 || this->kind() == 14 ? this->level->field_54 : this->level->field_50,
                &this->wreckGeometry->getMatrix());
            Globals::sound->play(20, Globals::options[15] ? &this->position() : nullptr, nullptr, 0.0f);
            this->level->field_74->enableSystemEmit(
                this->kind() == 0x37a3 || this->kind() == 14 ? this->level->field_54 : this->level->field_50, true);
            this->explosion = new Explosion(0);
            this->explosion->addFireStreaks();
            this->explosion->start(this->wreckGeometry->getPosition(), Vector{0.0f, 0.0f, 0.0f});
        } else {
            this->level->field_74->systemSetMatrix(
                this->kind() == 0x37a3 || this->kind() == 14 ? this->level->field_54 : this->level->field_50,
                &this->geometry->getMatrix());
            Globals::sound->play(20, Globals::options[15] ? &this->position() : nullptr, nullptr, 0.0f);
            this->level->field_74->enableSystemEmit(
                this->kind() == 0x37a3 || this->kind() == 14 ? this->level->field_54 : this->level->field_50, true);
            this->explosion = new Explosion(0);
            this->explosion->addFireStreaks();
            this->explosion->start(this->geometry->getPosition(), Vector{0.0f, 0.0f, 0.0f});
        }

        if (this->kind() == 14) {
            for (unsigned int i = 0; i < this->level->getEnemies()->size(); i++) {
                if ((*this->level->getEnemies())[i]->field_0x3e)
                    (*this->level->getEnemies())[i]->player->damage(9999999);
            }
            if (this->kind() == 14 && !this->player->destroyedByte) {
                Globals::status->field_118++;
                if (!Globals::achievements->hasMedal(39, 1)
                    && (int) ((float) Globals::status->field_118 / (float) Globals::achievements->getValue(39, 1)) <= 1) {
                    Hud *hud = this->level->getPlayer()->getHUD();
                    hud->hudEventMedal(39, (int) ((float) Globals::status->field_118
                                                  / (float) Globals::achievements->getValue(39, 1) * 100.0f));
                }
            }
        }
    }

    if (this->state == 5) {
        Array<Player *> *enemies = this->player->getEnemies();
        if (enemies) {
            this->targetEnemy = 0;
            for (unsigned int i = 0; i < enemies->size(); i++) {
                if (!(*enemies)[i]->isActive())
                    continue;
                this->targetPos() = (*enemies)[i]->getPosition();
                float dx = this->position().x - this->targetPos().x;
                if (!(dx < 50000.0f && dx > -50000.0f))
                    continue;
                float dy = this->position().y - this->targetPos().y;
                if (!(dy < 50000.0f && dy > -50000.0f))
                    continue;
                float dz = this->position().z - this->targetPos().z;
                if (dz < 50000.0f && dz > -50000.0f) {
                    Player *enemy = this->player->getEnemy(i);
                    this->targetEnemy = (int32_t) (intptr_t) enemy;
                    this->targetPos() = enemy->getPosition();
                    this->homingTarget.x = this->targetPos().x;
                    this->homingTarget.y = this->targetPos().y;
                    this->homingTarget.z = this->targetPos().z;
                    break;
                }
            }
        }
        float vx = this->homingTarget.x - this->position().x;
        float vy = this->homingTarget.y - this->position().y;
        float vz = this->homingTarget.z - this->position().z;
        this->homingDir.x = vx;
        this->homingDir.y = vy;
        this->homingDir.z = vz;
        if (vy < 50000.0f && vy > -50000.0f && vz < 50000.0f && vz > -50000.0f && vx > -50000.0f && vx < 50000.0f) {
            this->state = 1;
            this->player->setActive(true);
        }
    } else if (this->state == 4) {
        this->explosionElapsed += dt;
        if (this->explosion)
            this->explosion->update(dt, (TargetFollowCamera *) nullptr);
        this->explosionTimer() += dt;
        if (this->hasCargo && this->player->isActive() && this->secondaryGeometry()) {
            float angle = (float) (int) ((float) (dt >> 1) / 65536.0f * 6.2831855f);
            this->secondaryGeometry()->rotate(angle, angle, angle);
            if (this->explosionTimer() > 60000) {
                this->setActive(false);
                this->finished() = 1;
            }
        } else if (this->explosion && !this->explosion->isPlaying()) {
            if (this->explosionTimer() > 60000)
                this->setActive(false);
            this->finished() = 1;
        }
        if (this->wreckType >= 0) {
            if (this->wreckCollision && this->explosionElapsed > 140 && this->wreckMaterial < 0) {
                this->position() = this->geometry->getPosition();
                for (unsigned int i = 0; i < this->wreckCollision->size(); i++)
                    (*this->wreckCollision)[i]->update(this->position().x, this->position().y, this->position().z);
                switch (this->wreckType) {
                    case 0: this->wreckMaterial = 0x824c; break;
                    case 1: this->wreckMaterial = 0x8248; break;
                    case 2: this->wreckMaterial = 0x8249; break;
                    case 3: this->wreckMaterial = 0x824a; break;
                    case 4: this->wreckMaterial = 0x824b; break;
                    default: this->wreckMaterial = 0x824d; break;
                }
                unsigned int materialId;
                Globals::Canvas->MaterialCreate((unsigned short) this->wreckMaterial, materialId);
                Globals::Canvas->MeshChangeMaterial(this->wreckGeometry->meshId, materialId);
            }
            if (this->level->getPlayer() && this->level->getPlayer()->getTargetFollowCamera()
                && this->rumbleTimer > 0) {
                this->rumbleTimer += dt;
                if (this->rumbleTimer >= 2000)
                    this->rumbleTimer = 2000;
                this->level->getPlayer()->getTargetFollowCamera()->setRumblePercentage(
                    this->rumblePercentage * ((float) this->rumbleTimer / -2000.0f + 1.0f), 50);
                if (this->explosion && !this->explosion->isPlaying()) {
                    this->level->getPlayer()->getTargetFollowCamera()->setRumblePercentage(0.0f, 0);
                    this->rumbleTimer = 0;
                }
            }
        }
    } else if (this->state == 3) {
        if (this->explosion)
            this->explosion->update(dt, (TargetFollowCamera *) nullptr);
        if (this->kind() != 0x37a3) {
            if (this->moving) {
                this->intPosZ += dt;
                this->wreckGeometry->moveForward((float) dt);
                if (this->secondaryGeometry())
                    this->secondaryGeometry()->moveForward((float) dt);
            }
            reinterpret_cast<Matrix &>(this->player->transform[0]) = this->wreckGeometry->getMatrix();
            this->position() = this->wreckGeometry->getPosition();
            if (this->boundingVolumes) {
                for (unsigned int i = 0; i < this->boundingVolumes->size(); i++)
                    (*this->boundingVolumes)[i]->update(this->position().x, this->position().y,
                                                        this->position().z);
            }
        }
        this->finished() = 0;
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->wreckGeometry->transform))->Update(dt, false);
        if (!((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(this->wreckGeometry->transform))->animating) {
            this->state = 4;
            this->level->field_74->enableSystemEmit(this->kind() == 0x37a3 || this->kind() == 14 ? this->level->field_54 : this->level->field_50, false);
            this->explosion->reset();
            float scale = 6.0f;
            if (this->kind() == 0x37e7)
                scale = 8.0f;
            if (this->kind() == 0x37a3)
                scale = 8.0f;
            this->explosion->setScaling(scale);
            this->explosion->start(this->position(), Vector{0.0f, 0.0f, 0.0f});
            this->explosionElapsed = 0;
            this->rumbleTimer = 1;
            if (this->level->getPlayer() && this->level->getPlayer()->getTargetFollowCamera()) {
                float distance = AbyssEngine::AEMath::VectorLength(
                    this->position() - *this->level->getPlayer()->getTargetFollowCamera()->getPosition());
                this->rumblePercentage = 1.0f - (distance < 30000.0f ? distance : 30000.0f) / 30000.0f;
                this->level->getPlayer()->getTargetFollowCamera()->setRumblePercentage(this->rumblePercentage, 50);
            }
        }
    }

    this->player->mirrorPosX = this->intPosX;
    this->player->mirrorPosY = this->intPosY;
    this->player->mirrorPosZ = this->intPosZ;
}

void PlayerFixedObject::setExhaustVisible(bool v) {
    AEGeometry *geom = (AEGeometry *) this->geometry;
    if (geom != nullptr && (int) geom->childTransform != -1)
        ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(geom->childTransform))->SetVisible(v);
}

void PlayerFixedObject::render() {
    if (this->secondaryGeometry() != nullptr && this->shipHidden == 0)
        this->secondaryGeometry()->render();
    switch (this->state) {
        case 3:
            if (this->shipHidden == 0)
                this->wreckGeometry->render();
            this->explosion->render();
            return;
        case 4:
            if (this->shipHidden == 0)
                this->wreckGeometry->render();
            if (this->explosion != nullptr)
                this->explosion->render();
            return;
        case 5:
            break;
        default:
            if (!this->player->isActive())
                return;
            break;
    }
    if (this->shipHidden == 0)
        ((AEGeometry *) this->geometry)->render();
}

int PlayerFixedObject::collide(float x, float y, float z) {
    PlayerFixedObject *self = this;
    Array<BoundingVolume *> *a = self->wreckCollision;
    if ((a != 0 || self->state != 4) && self->collisionEnabled() != 0) {
        if (a != 0 && self->state == 4) {
            for (uint32_t i = 0; i < a->size(); i++) {
                BoundingVolume *bv = (*a)[i];
                if (bv->collide(x, y, z) != 0) return 1;
                a = self->wreckCollision;
            }
        } else {
            Array<BoundingVolume *> *b = self->boundingVolumes;
            if (b != 0) {
                for (uint32_t i = 0; i < b->size(); i++) {
                    BoundingVolume *bv = (*b)[i];
                    if (bv->collide(x, y, z) != 0) return 1;
                    b = self->boundingVolumes;
                }
            }
        }
    }
    return 0;
}

V3 PlayerFixedObject::getProjectionVector(const Vector &vec) {
    PlayerFixedObject *self = this;
    Array<BoundingVolume *> *bv = self->wreckCollision;
    if (bv != 0 && self->state == 4) {
        int idx = self->collisionIndex;
        return (*bv)[idx]->getProjectionVector(vec);
    }
    Array<BoundingVolume *> *bv2 = self->boundingVolumes;
    if (bv2 != 0) {
        int idx = self->collisionIndex;
        return (*bv2)[idx]->getProjectionVector(vec);
    }
    V3 z = {0.0f, 0.0f, 0.0f};
    return z;
}

V3 PlayerFixedObject::projectCollisionOnSurface(const Vector &vec) {
    Array<BoundingVolume *> *bv = this->wreckCollision;
    if (bv != 0 && this->state == 4) {
        return BoundingVolume::staticProjectCollisionOnSurface(vec, bv);
    }
    Array<BoundingVolume *> *bv2 = this->boundingVolumes;
    if (bv2 != 0) {
        return BoundingVolume::staticProjectCollisionOnSurface(vec, bv2);
    }
    V3 z = {0.0f, 0.0f, 0.0f};
    return z;
}

void PlayerFixedObject::outerCollide(Vector v) {
    this->outerCollide(v.x, v.y, v.z);
}

int PlayerFixedObject::outerCollide(float x, float y, float z) {
    PlayerFixedObject * self = this;
    Array<BoundingVolume *> *a = self->wreckCollision;
    if ((a != 0 || self->state != 4) && self->collisionEnabled() != 0) {
        if (a != 0 && self->state == 4) {
            for (uint32_t i = 0; i < a->size(); i++) {
                BoundingVolume *bv = (*a)[i];
                if (bv->outerCollide(x, y, z) != 0) {
                    self->collisionIndex = i;
                    return 1;
                }
                a = self->wreckCollision;
            }
        } else {
            Array<BoundingVolume *> *b = self->boundingVolumes;
            if (b != 0) {
                for (uint32_t i = 0; i < b->size(); i++) {
                    BoundingVolume *bv = (*b)[i];
                    if (bv->outerCollide(x, y, z) != 0) {
                        self->collisionIndex = i;
                        return 1;
                    }
                    b = self->boundingVolumes;
                }
            }
        }
    }
    return 0;
}

void PlayerFixedObject::setDockingType(int v) {
    this->dockingType = v;
}

int PlayerFixedObject::getDockingType() {
    return this->dockingType;
}

void PlayerFixedObject::setTransportID(int v) {
    this->transportID = v;
}

int PlayerFixedObject::getTransportID() {
    return this->transportID;
}

String PlayerFixedObject::getName() {
    return this->name;
}

void PlayerFixedObject::setName(String name) {
    this->name = name;
}

void PlayerFixedObject::hideShip() {
    this->shipHidden = 1;
}

void PlayerFixedObject::setDeadButSelectable() {
    Player *player = this->player;
    this->moving = 0;
    player->setHitpoints(1);
    ((Player *) (this->player))->setVulnerable(false);
    this->level->lodManager->removeObject((AEGeometry *) this->geometry);
    AEGeometry *geom = this->geometry;
    if (geom != 0) delete geom;
    AEGeometry *newGeom = this->wreckGeometry;
    this->geometry = newGeom;
    AbyssEngine::Transform *t = (AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(newGeom->transform);
    t->SetAnimationRangeInTime(t->animationLength, t->animationLength);
}
