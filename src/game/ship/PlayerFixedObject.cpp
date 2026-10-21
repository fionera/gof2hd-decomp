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

int PlayerFixedObject::getDockingType() {
    return this->dockingType;
}

void PlayerFixedObject::setBV(Array<BoundingVolume *> *bv) {
    this->boundingVolumes = bv;
}

void PlayerFixedObject::hideShip() {
    this->shipHidden = 1;
}

int PlayerFixedObject::getTransportID() {
    return this->transportID;
}

void PlayerFixedObject::setDockingType(int v) {
    this->dockingType = v;
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

String PlayerFixedObject::getName() {
    return this->name;
}

void PlayerFixedObject::setName(String name) {
    this->name = name;
}

void PlayerFixedObject::setMoving(bool v) {
    this->moving = v;
}

V3 PlayerFixedObject::projectCollisionOnSurface(const Vector &vec) {
    Array<BoundingVolume *> *bv = this->wreckCollision;
    if (bv != 0 && this->state == 4) {
        V3 result;
        reinterpret_cast<BoundingVolume *>(&result)->staticProjectCollisionOnSurface(vec, bv);
        return result;
    }
    Array<BoundingVolume *> *bv2 = this->boundingVolumes;
    if (bv2 != 0) {
        V3 result;
        reinterpret_cast<BoundingVolume *>(&result)->staticProjectCollisionOnSurface(vec, bv2);
        return result;
    }
    V3 z = {0.0f, 0.0f, 0.0f};
    return z;
}

void PlayerFixedObject::setTransportID(int v) {
    this->transportID = v;
}

void PlayerFixedObject::outerCollide(Vector v) {
    this->outerCollide(v.x, v.y, v.z);
}

V3 PlayerFixedObject::getPosition() {
    float fy = (float) intPosY;
    float fx = (float) intPosX;
    float fz = (float) intPosZ;
    V3 pos = {fx, fy, fz};
    return pos;
}

void PlayerFixedObject::update(int dt) {
    this->deltaTime = dt;
    this->player->update(dt, this->aiActiveCounter() != -1 && this->moving);

    if ((this->faction() & ~1) == 8)
        this->player->enemyFlagsLo = 1;
    else
        this->player->enemyFlagsLo = ((Standing *) (intptr_t) Globals::status->getStanding())->isEnemy(this->faction());
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
        this->moving = 0;
        this->state = 3;
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
                    this->homingTarget = this->targetPos();
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
        if (vx < 50000.0f && vz > -50000.0f && vx > -50000.0f && vy < 50000.0f && vy > -50000.0f && vz < 50000.0f) {
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
                unsigned short material;
                switch (this->wreckType) {
                    case 0: material = 0x824c; break;
                    case 1: material = 0x8248; break;
                    case 2: material = 0x8249; break;
                    case 3: material = 0x824a; break;
                    case 4: material = 0x824b; break;
                    default: material = 0x824d; break;
                }
                this->wreckMaterial = material;
                unsigned int materialId;
                Globals::Canvas->MaterialCreate(material, materialId);
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
            this->rumbleTimer = 1;
            this->explosionElapsed = 0;
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

void PlayerFixedObject::setBV(BoundingVolume *bv) {
    Array<BoundingVolume *> *a = new Array<BoundingVolume *>();
    this->boundingVolumes = a;
    ArrayAdd(bv, *a);
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

void PlayerFixedObject::setWreckedMeshId(int meshId) {
    this->wreckMeshId = (uint16_t) meshId;
    AEGeometry *geom = new AEGeometry((uint16_t) meshId, Globals::Canvas, true);
    this->wreckGeometry = geom;
    AbyssEngine::Transform *t = (AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(geom->transform);
    *(int *) &t->boundingRadius = 0x48f42400;

    int kind = this->kind();
    int sel;
    if (kind == 0xd) {
        sel = 4;
    } else if (kind == 0xe) {
        sel = 0;
    } else if (kind == 0xf) {
        if (this->faction() == 3) sel = 1;
        else sel = (this->faction() == 2) ? 2 : 3;
    } else if (kind == 0x37a3) {
        sel = 5;
    } else {
        sel = this->wreckType;
        if (sel < 0) return;
        this->wreckCollision = Globals::globals->getWreckCollision(sel, this->wreckGeometry);
        return;
    }
    this->wreckType = sel;
    this->wreckCollision = Globals::globals->getWreckCollision(sel, this->wreckGeometry);
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

PlayerFixedObject::~PlayerFixedObject() {
    PlayerFixedObject *self = this;
    AEGeometry *wreck = self->wreckGeometry;
    if (wreck != self->geometry) {
        if (wreck != 0) delete wreck;
        self->wreckGeometry = 0;
    }
    Array<BoundingVolume *> *bvB = self->boundingVolumes;
    if (bvB != 0) {
        ArrayReleaseClasses(*bvB);
        delete bvB;
    }
    self->boundingVolumes = 0;
    Array<BoundingVolume *> *bvA = self->wreckCollision;
    if (bvA != 0) {
        ArrayReleaseClasses(*bvA);
        delete bvA;
    }
    self->wreckCollision = 0;
    Explosion *expl = self->explosion;
    if (expl != 0) delete expl;
    self->explosion = 0;
    {
        if (self->name.data) delete[] self->name.data;
        self->name.data = nullptr;
        self->name.length = 0;
    }
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

static const int g_pfo_stationIdx[4] = {0, 0, 0, 0};

static const int g_pfo_lootParams[8] = {0, 0, 0, 0, 0, 0, 0, 0};

PlayerFixedObject::PlayerFixedObject(int kind, int param2, Player *player, AEGeometry *geom,
                                     float x, float y, float z)
    : KIPlayer(kind, -1, player, geom, x, y, z, false) {
    PlayerFixedObject * self = this;

    Vector zeroVec = {0.0f, 0.0f, 0.0f};
    self->respawnPos = zeroVec;
    self->homingTarget = zeroVec;
    self->homingDir = zeroVec;
    self->field_0x15c = 0;
    self->field_0x160 = 0;

    {
        if (self->name.data) delete[] self->name.data;
        self->name.data = nullptr;
        self->name.length = 0;
    }
    self->explosion = 0;
    self->wreckMeshId = 0;
    self->faction() = param2;
    self->wreckGeometry = 0;
    self->boundingVolumes = 0;
    self->wreckCollision = 0;
    self->deltaTime = 0;
    self->targetEnemy = 0;
    self->collisionIndex = 0;
    self->field_0x170 = 0;
    self->shipHidden = 0;
    self->explosionElapsed = 0;
    self->field_0x174 = 0;
    self->intPosX = 0;
    self->intPosY = 0;
    self->intPosZ = 0;

    Vector p = {x, y, z};
    self->position() = p;

    self->moving = 0;
    self->wreckType = -1;
    self->wreckMaterial = -1;
    self->dockingType = 0;
    self->intPosX = (int32_t) x;
    self->intPosY = (int32_t) y;
    self->intPosZ = (int32_t) z;

    {
        String tmp("", false);
        self->name = tmp;
    }

    Mission *mission = Globals::status->getMission();
    int campaign = mission->isCampaignMission();
    bool special = false;
    if (campaign != 0) {
        int cm = Globals::status->getCurrentCampaignMission();
        if (cm == 0x28) special = true;
        else {
            cm = Globals::status->getCurrentCampaignMission();
            if (cm == 0x29) special = true;
        }
    }

    if (special) {
        if (self->lootList() != 0) {
            delete self->lootList();
        }
        self->lootList() = 0;
    } else {
        Generator *gen = new Generator();
        if (kind == 0x37a3) {
            self->field_0x41() = 1;
            Station *station = Globals::status->getStation();
            int sidx = station->getIndex();
            for (uint32_t i = 0; i < 4; i++) {
                if (g_pfo_stationIdx[i] == sidx) {
                    self->lootList() = gen->getLootList(g_pfo_lootParams[i * 2 + 1], 0);
                }
            }
        } else {
            Array<int> *loot = gen->getLootList(-1, -1);
            self->lootList() = loot;
            if (loot != 0) {
                int second = (kind != 0x498e) ? 0x4a88 : 0x498e;
                if (kind != 0x498e && kind != second) {
                    for (int idx = 1; (uint32_t)(idx - 1) < self->lootList()->size(); idx += 2) {
                        if (kind == 0xe) {
                            int r = Globals::rnd->nextInt();
                            int &cell = (*self->lootList())[idx];
                            cell = cell * (r + 5);
                        } else {
                            int r = Globals::rnd->nextInt();
                            int &base = (*self->lootList())[idx];
                            base = base * (r + 2);
                            int r2 = Globals::rnd->nextInt();
                            int &cell = (*self->lootList())[idx];
                            if (cell < r2 + 8) cell = r2 + 8;
                        }
                    }
                }
            }
        }
        delete gen;
    }

    ((Player *) self->player)->spawnedFlag = 1;
    if (kind != 0x37a3) {
        self->aiActiveCounter() = 0x2f;
        if (kind == 0xe) {
            self->aiActiveCounter() = -1;
            self->moving = 0;
        }
    }
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

void PlayerFixedObject::moveForward(int amount) {
    float d = (float) amount;
    this->intPosZ = amount + this->intPosZ;
    ((AEGeometry *) (this->geometry))->moveForward(d);
    const Matrix *m = &((AEGeometry *) (this->geometry))->getMatrix();
    *(Matrix *) ((Player *) this->player)->transform = *m;
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
