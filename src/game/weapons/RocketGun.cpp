
#include "game/weapons/RocketGun.h"
#include "game/core/Globals.h"
#include "engine/render/AEGeometry.h"
#include "game/world/Level.h"
#include "engine/render/ParticleSystemManager.h"
#include "game/ship/PlayerEgo.h"
#include "game/weapons/Gun.h"
#include "game/ship/Player.h"
#include "game/weapons/Radar.h"
#include "engine/render/PaintCanvas.h"

void MatrixRotateVector(Vector &out, const Matrix &matrix, const Vector &vec);

static inline void MatrixGetDir(Vector &out, const Matrix &matrix) { out = AbyssEngine::AEMath::MatrixGetDir(matrix); }

static inline void VectorCross(Vector &out, const Vector &a, const Vector &b) { out = AbyssEngine::AEMath::VectorCross(a, b); }

static inline void VectorNormalize(Vector &out, const Vector &in) { out = AbyssEngine::AEMath::VectorNormalize(in); }

void VectorRotateToTarget(Vector &out, const Vector &in);

static inline void VectorScale(Vector &out, const Vector &in, float scale) { out.x=in.x*scale; out.y=in.y*scale; out.z=in.z*scale; }

namespace AbyssEngine {
    namespace AEMath {
        Vector operator+(const Vector &a, const Vector &b);

        Vector operator-(const Vector &a, const Vector &b);

        Vector operator*(const Vector &v, float scale);
    }
}





static const float kRocketTurnRate = 0.3f;




static const float kMuzzleZAdd = -100.0f;
static const float kScaleDiv = -200.0f;
static const float kScaleMul = 255.0f;
static const float kZeroCompare = 50000.0f;
static const float kWave = 0.003f;


int RocketGun::isRocketGun() { return 1; }

RocketGun::RocketGun(int meshVariantId, Gun *gun, int mesh, int /*unused4*/,
                     uint32_t flags, int rocketKind, bool homing, Level *level)
    : ObjectGun(meshVariantId, gun, mesh, flags, level) {
    this->steerX = 0;
    this->steerY = 0;
    this->steerZ = 0;
    this->homing = homing;
    this->activeShotId = -1;
    this->turnRate = kRocketTurnRate;
    this->particleSystem = -1;
    this->rocketKind = rocketKind;
    this->fadeTimer = 0;
    this->trailMatrices = nullptr;
    this->trailSystems = nullptr;
    this->trailTimers = nullptr;
    this->particleManager = nullptr;
    this->unusedSlot = -1;

    int gate = 0x37a9;
    if (mesh != 0x37a9)
        gate = 0x37a7;
    if (mesh == 0x37a9 || mesh == gate) {
        uint16_t meshId = 0x37aa;
        if (meshVariantId == 0x37a7)
            meshId = 0x37a8;
        AEGeometry *geom = new AEGeometry(meshId, Globals::Canvas, false);
        Globals::Canvas->TransformAddChild(this->transform, geom->transform);
        delete geom;
    }
}

RocketGun::~RocketGun() {
    delete this->trailMatrices;
    this->trailMatrices = nullptr;

    delete this->trailSystems;
    this->trailSystems = nullptr;

    delete this->trailTimers;
    this->trailTimers = nullptr;
}

void RocketGun::setRadar(Radar *radar) {
    this->radar = radar;
    this->particleManager = radar->level->field_80;

    int item = this->gun->itemIndex;
    int trailMode = item - 0x1c; // 0..2: the three missile tiers
    if ((unsigned int) trailMode < 3 || item == 0xc1 || this->gun->weaponType == ITEM_SORT_CLUSTER_MISSILE) {
        if (this->gun->weaponType == ITEM_SORT_CLUSTER_MISSILE)
            trailMode = 0;
        else if (item == 0xc1)
            trailMode = 3;

        this->trailMatrices = new Array<Matrix>();
        this->trailSystems = new Array<int>();
        this->trailTimers = new Array<int>();
        ArraySetLength(this->gun->count, *this->trailMatrices);
        ArraySetLength(this->gun->count, *this->trailSystems);
        ArraySetLength(this->gun->count, *this->trailTimers);

        ParticleSettings::ParticleSet defaultSet =
                trailMode == 2 ? ParticleSettings::ParticleSet_0x1b : ParticleSettings::ParticleSet_0x1c;
        for (uint32_t i = 0; i < this->gun->count; ++i) {
            ParticleSystemManager *manager;
            if (this->gun->itemIndex == 0xc1) {
                this->particleManager = radar->level->field_98;
                manager = this->particleManager;
            } else {
                manager = this->particleManager;
            }
            ParticleSettings::ParticleSet set;
            if (trailMode == 0)
                set = ParticleSettings::ParticleSet_0x19;
            else if (trailMode == 1)
                set = ParticleSettings::ParticleSet_0x1a;
            else
                set = defaultSet;
            (*this->trailSystems)[i] = manager->addSystem(&(*this->trailMatrices)[i], set, false);
            this->particleManager->enableSystemEmit((*this->trailSystems)[i], false);
            (*this->trailTimers)[i] = 0;
        }
        return;
    }

    if ((unsigned int) (this->rocketKind - 4) < 2 || this->rocketKind == 0x28) {
        this->trailMatrices = new Array<Matrix>();
        this->trailSystems = new Array<int>();
        this->trailTimers = new Array<int>();
        ArraySetLength(this->gun->count, *this->trailMatrices);
        ArraySetLength(this->gun->count, *this->trailSystems);
        ArraySetLength(this->gun->count, *this->trailTimers);
        for (uint32_t i = 0; i < this->gun->count; ++i) {
            (*this->trailSystems)[i] = radar->level->field_80->addSystem(
                    &(*this->trailMatrices)[i], ParticleSettings::ParticleSet_0x27, false);
            radar->level->field_80->enableSystemEmit((*this->trailSystems)[i], false);
            (*this->trailTimers)[i] = 0;
        }
        return;
    }

    if (item == 0xe8) {
        this->particleSystem = radar->level->field_9c->addSystem(
                (Matrix *) Globals::Canvas->TransformGetLocal(this->transform), ParticleSettings::ParticleSet_0x2f,
                false);
        radar->level->field_9c->enableSystemEmit(this->particleSystem, false);
    } else {
        this->particleSystem = radar->level->particleRenderBoolPtr->addSystem(
                (Matrix *) Globals::Canvas->TransformGetLocal(this->transform), ParticleSettings::ParticleSet_0xc,
                false);
        radar->level->particleRenderBoolPtr->enableSystemEmit(this->particleSystem, false);
    }
}

void RocketGun::render() {
    ObjectGun::render();
}

void RocketGun::update(int elapsed) {
    Vector norm;
    Matrix matrix;
    Vector axis;
    Vector dir;
    Vector gunVec;
    Vector zero = {0.0f, 0.0f, 0.0f};

    Gun *gun = this->gun;
    gun->update(elapsed);

    if (this->hasGeometry != 0 && gun->delayActive != 0) {
        PlayerEgo *player = (PlayerEgo *) (long) this->level->getPlayer();
        Vector basePos = player->getPosition();
        gunVec.x = gun->offset.x;
        gunVec.y = gun->offset.y;
        gunVec.z = gun->offset.z + kMuzzleZAdd;
        MatrixRotateVector(axis, reinterpret_cast<AbyssEngine::AEMath::Matrix &>(player->field_0x4), gunVec);
        basePos += axis;
        this->geometry->setPosition(basePos);

        float scaling = (float) gun->delayTimer;
        scaling = (float) (int) ((scaling / kScaleDiv + 1.0f) * kScaleMul);
        scaling = scaling / kScaleMul;
        this->geometry->setScaling(scaling);

        MatrixGetDir(axis, reinterpret_cast<AbyssEngine::AEMath::Matrix &>(player->field_0x4));
        zero.x = 0.0f;
        zero.y = 1.0f;
        zero.z = 0.0f;
        this->geometry->setDirection(axis, zero);
    }

    gun = this->gun;
    this->wasFiring = gun->delayActive;

    if (gun->hitSmall != 0) {
        this->activeShotId = -1;
        this->fadeTimer = 0;
        gun->hitSmall = 0;

        Matrix *local = (Matrix *) Globals::Canvas->TransformGetLocal(this->transform);
        int shot = gun->fireIndex;
        Vector *positions = (Vector *) gun->positions;
        MatrixSetTranslation(matrix, positions[shot].x, positions[shot].y, positions[shot].z);

        Vector *velocities = (Vector *) gun->velocities;
        gunVec = velocities[shot];

        local = (Matrix *) Globals::Canvas->TransformGetLocal(this->transform);
        memcpy(&matrix, local, 0x3c);
        axis.x = 0.0f;
        axis.y = 1.0f;
        axis.z = 0.0f;
        VectorCross(dir, axis, gunVec);
        VectorNormalize(norm, dir);
        VectorRotateToTarget(dir, norm);
        VectorCross(norm, gunVec, dir);
        VectorRotateToTarget(axis, norm);
        VectorNormalize(norm, axis);
        VectorRotateToTarget(axis, norm);

        matrix.m[0] = dir.x;
        matrix.m[1] = axis.x;
        matrix.m[2] = gunVec.x;
        matrix.m[4] = dir.y;
        matrix.m[5] = axis.y;
        matrix.m[6] = gunVec.y;
        matrix.m[8] = dir.z;
        matrix.m[9] = axis.z;
        matrix.m[10] = gunVec.z;
        Globals::Canvas->TransformSetLocal(this->transform, matrix);

        if (this->trailMatrices == nullptr) {
            int kind = this->rocketKind;
            if ((uint32_t)(kind - 4) < 2 || kind == 0x28) {
                ParticleSystemManager *manager =
                        ((Level *) this->radar->level)->field_80;
                manager->enableSystemRender(this->particleSystem, true);
                manager->enableSystemEmit(this->particleSystem, true);
            } else {
                ParticleSystemManager *manager;
                if (gun->itemIndex == 0xe8)
                    manager = ((Level *) this->radar->level)->field_9c;
                else
                    manager = ((Level *) this->radar->level)->particleRenderBoolPtr;
                manager->enableSystemEmit(this->particleSystem, this->particleSystem != 0);
            }
        } else {
            int shotIndex = gun->fireIndex;
            Vector *shotPositions = (Vector *) gun->positions;
            int system = (*this->trailSystems)[shotIndex];
            ParticleSystemManager *manager = this->particleManager;
            if (shotPositions[shotIndex].z != kZeroCompare) {
                (*this->trailMatrices)[shotIndex] = matrix;
                manager->resetSystem(system);
                manager->enableSystemEmit(system, true);
                manager->enableSystemRender(system, true);
                (*this->trailTimers)[shotIndex] = 0;
            } else {
                manager->enableSystemEmit(system, false);
                manager->resetSystem(system);
                (*this->trailTimers)[gun->fireIndex] = 0;
            }
        }

        gun = this->gun;
        if (gun->weaponType == ITEM_SORT_CLUSTER_MISSILE && gun->fireIndex > 0) {
            gun->hitSmall = 1;
            gun->fireIndex = gun->fireIndex - 1;
            this->update(elapsed);
            this->gun->update(elapsed);
            gun = this->gun;
        }
    }

    Vector *positions = (Vector *) gun->positions;
    if (gun->active != 0 && gun->count == 1 && positions[0].z == kZeroCompare) {
        gun->active = 0;
        int kind = this->rocketKind;
        ParticleSystemManager *manager;
        if ((uint32_t)(kind - 4) < 2 || kind == 0x28) {
            if (this->fadeTimer == 0)
                this->fadeTimer = 2000;
            manager = ((Level *) this->radar->level)->field_80;
        } else {
            if (gun->itemIndex == 0xe8)
                manager = ((Level *) this->radar->level)->field_9c;
            else
                manager = ((Level *) this->radar->level)->particleRenderBoolPtr;
        }
        manager->enableSystemEmit(this->particleSystem, false);
    } else if (this->fadeTimer > 0 && gun->count == 1 && positions[0].z == kZeroCompare) {
        int timer = this->fadeTimer - elapsed;
        this->fadeTimer = timer;
        if (timer < 1) {
            this->fadeTimer = 0;
            this->particleManager
                    ->enableSystemRender(this->particleSystem, false);
        }
    }

    float dt = (float) elapsed;
    for (uint32_t i = 0; i < this->gun->count; i++) {
        gun = this->gun;
        Vector *gunPositions = (Vector *) gun->positions;
        Vector *gunVelocities = (Vector *) gun->velocities;

        axis = gunVelocities[i] * dt;
        gunVec = axis * 0.5f;
        Vector advanced = gunPositions[i] + gunVec;
        zero = advanced;

        gun = this->gun;
        if (gun->active != 0) {
            if (zero.z != kZeroCompare) {
                if (this->homing != 0 &&
                    (this->trailMatrices != nullptr ||
                     gun->lifetimes[i] < gun->initialLifetime - 1000)) {
                    this->seekEnemy(gun->lifetimes[i], i);
                    gun = this->gun;
                }

                if (gun->weaponType == ITEM_SORT_CLUSTER_MISSILE) {
                    uint32_t total = (uint32_t) gun->initialLifetime;
                    uint32_t base = ((unsigned) (total * i) / (unsigned) (gun->count));
                    float waveIn = (float) base +
                                   (float) (gun->initialLifetime - gun->lifetimes[i]);
                    float s = AbyssEngine::AEMath::Sinf(waveIn * kWave);
                    float c = AbyssEngine::AEMath::Cosf(waveIn * kWave);
                    Vector *up = (Vector *) gun->upVectors;
                    Vector cross;
                    VectorCross(gunVec, gunVelocities[i], up[i]);
                    VectorNormalize(cross, gunVec);
                    VectorScale(axis, cross, s + s);
                    VectorScale(gunVec, axis, dt);
                    ((Vector *) gun->positions)[i] += gunVec;
                    VectorScale(axis, up[i], c + c);
                    VectorScale(gunVec, axis, dt);
                    ((Vector *) gun->positions)[i] += gunVec;
                }
            } else {
                if (this->trailMatrices != nullptr) {
                    int system = (*this->trailSystems)[i];
                    this->particleManager
                            ->enableSystemEmit(system, false);
                    int *timer = &(*this->trailTimers)[i];
                    if (*timer == 0)
                        *timer = 2000;
                }
            }

            if (this->trailMatrices != nullptr) {
                gun = this->gun;
                gunPositions = (Vector *) gun->positions;
                gunVelocities = (Vector *) gun->velocities;
                axis = gunVelocities[i] * dt;
                gunVec = axis * 0.5f;
                advanced = gunPositions[i] + gunVec;
                zero = advanced;
                Matrix &trail = (*this->trailMatrices)[i];
                MatrixSetTranslation(trail, zero.x, zero.y, zero.z);
                VectorNormalize(gunVec, gunVelocities[i]);
                trail.m[2] = gunVec.x;
                trail.m[6] = gunVec.y;
                trail.m[10] = gunVec.z;
            }
        }

        if (this->trailTimers != nullptr && zero.z == kZeroCompare) {
            int *timer = &(*this->trailTimers)[i];
            if (*timer > 0) {
                int left = *timer - elapsed;
                *timer = left;
                if (left < 1) {
                    *timer = 0;
                    this->particleManager
                            ->enableSystemRender((*this->trailSystems)[i], false);
                }
            }
        }
    }
}

void RocketGun::seekEnemy(int /*unused*/, int index) {
    Array<Player *> *enemies = (Array<Player *> *) this->gun->getEnemies();
    Player *enemy;
    if (this->gun->owner != nullptr && this->gun->isPlayerGun() == 0 && this->gun->owner != nullptr &&
        this->gun->owner->getKIPlayer()->field_0x38 >= 0 && this->gun->owner->getEnemies() != nullptr) {
        Player *owner = this->gun->owner;
        enemy = owner->getEnemy(owner->getKIPlayer()->field_0x38);
    } else {
        if (enemies == nullptr || this->radar == nullptr)
            return;
        KIPlayer *contact = this->radar->lockedEnemy;
        if (contact == nullptr || contact->field_0x76 == 0 || contact->field_0x74 != 0)
            return;
        if (((PlayerEgo *) (long) ((Level *) this->radar->level)->getPlayer())->isInFreeLookMode() != 0)
            return;
        enemy = this->radar->lockedEnemy->player;
    }
    if (enemy == nullptr)
        return;

    Vector &steer = *(Vector *) &this->steerX;
    steer = AbyssEngine::AEMath::VectorNormalize(enemy->getPosition() - ((Vector *) this->gun->positions)[index]);
    Vector heading = AbyssEngine::AEMath::VectorNormalize(((Vector *) this->gun->velocities)[index]);
    steer -= heading;
    steer /= this->turnRate * 20.0f;
    heading += steer;
    ((Vector *) this->gun->velocities)[index] = AbyssEngine::AEMath::VectorNormalize(heading);
    ((Vector *) this->gun->velocities)[index] *= this->gun->pitchRate;
}

void RocketGun::translate(const Vector & /*v*/) {
}

