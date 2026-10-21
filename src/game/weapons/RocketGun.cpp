
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

static inline void MatrixGetDir(Vector &out, const Matrix &matrix) { out = AbyssEngine::AEMath::MatrixGetDir(matrix); }

static inline void VectorCross(Vector &out, const Vector &a, const Vector &b) { out = AbyssEngine::AEMath::VectorCross(a, b); }

static inline void VectorNormalize(Vector &out, const Vector &in) { out = AbyssEngine::AEMath::VectorNormalize(in); }

namespace AbyssEngine {
    namespace AEMath {
        Vector operator+(const Vector &a, const Vector &b);

        Vector operator-(const Vector &a, const Vector &b);

        Vector operator*(const Vector &v, float scale);
    }
}

static const float kRocketTurnRate = 0.3f;

int RocketGun::isRocketGun() { return 1; }

RocketGun::RocketGun(int meshVariantId, Gun *gun, int mesh, int /*unused4*/,
                     uint32_t flags, int rocketKind, bool homing, Level *level)
    : ObjectGun(meshVariantId, gun, mesh, flags, level) {
    this->steerX = 0;
    this->steerY = 0;
    this->steerZ = 0;
    this->activeShotId = -1;
    this->particleSystem = -1;
    this->rocketKind = rocketKind;
    this->homing = homing;
    this->turnRate = kRocketTurnRate;
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
    using namespace AbyssEngine::AEMath;

    Vector ahead = {0.0f, 0.0f, 0.0f};
    this->gun->update(elapsed);

    if (this->hasGeometry != 0 && this->gun->delayActive != 0) {
        // Muzzle flash grows on the launcher while the launch delay runs down.
        PlayerEgo *player = this->level->getPlayer();
        Vector muzzle = player->getPosition();
        Vector barrel = this->gun->offset;
        barrel.z += -100.0f;
        const Matrix &ship = reinterpret_cast<const Matrix &>(player->player->transform[0]);
        muzzle += MatrixRotateVector(ship, barrel);
        this->geometry->setPosition(muzzle);
        const float scale = (float) (int) (((float) this->gun->delayTimer / -200.0f + 1.0f) * 255.0f) / 255.0f;
        this->geometry->setScaling(scale, scale, scale);
        const Vector forward = MatrixGetDir(reinterpret_cast<const Matrix &>(player->player->transform[0]));
        Vector up;
        up.y = 1.0f;
        up.x = 0.0f;
        up.z = 0.0f;
        this->geometry->setDirection(forward, up);
    }

    Gun *gun = this->gun;
    this->wasFiring = gun->delayActive;

    if (gun->hitSmall != 0) {
        // A rocket was just launched: orient the rocket mesh along its velocity.
        this->activeShotId = -1;
        this->fadeTimer = 0;
        gun->hitSmall = 0;

        PaintCanvas *canvas = Globals::Canvas;
        Matrix &local = *(Matrix *) canvas->TransformGetLocal(this->transform);
        const Vector &shot = ((const Vector *) this->gun->positions)[this->gun->fireIndex];
        MatrixSetTranslation(local, shot.x, shot.y, shot.z);
        Vector heading = ((const Vector *) this->gun->velocities)[this->gun->fireIndex];

        Matrix frame = *(const Matrix *) Globals::Canvas->TransformGetLocal(this->transform);
        Vector up;
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        Vector side = VectorCross(up, heading);
        side = VectorNormalize(side);
        up = VectorCross(heading, side);
        up = VectorNormalize(up);
        frame.m[0] = side.x;
        frame.m[1] = up.x;
        frame.m[2] = heading.x;
        frame.m[4] = side.y;
        frame.m[5] = up.y;
        frame.m[6] = heading.y;
        frame.m[8] = side.z;
        frame.m[9] = up.z;
        frame.m[10] = heading.z;
        Globals::Canvas->TransformSetLocal(this->transform, frame);

        if (this->trailMatrices == nullptr) {
            const int kind = this->rocketKind;
            if ((uint32_t) (kind - 4) < 2 || kind == 0x28) {
                this->radar->level->field_80->enableSystemRender(this->particleSystem, true);
                this->radar->level->field_80->enableSystemEmit(this->particleSystem, true);
            } else {
                Level *level = this->radar->level;
                if (this->gun->itemIndex == 0xe8)
                    level->field_9c->enableSystemEmit(this->particleSystem, true);
                else
                    level->particleRenderBoolPtr->enableSystemEmit(this->particleSystem, this->gun->itemIndex != 0xb3);
            }
        } else if (((const Vector *) this->gun->positions)[this->gun->fireIndex].z == 50000.0f) {
            this->particleManager->enableSystemEmit((*this->trailSystems)[this->gun->fireIndex], false);
            this->particleManager->resetSystem((*this->trailSystems)[this->gun->fireIndex]);
            (*this->trailTimers)[this->gun->fireIndex] = 0;
        } else {
            (*this->trailMatrices)[this->gun->fireIndex] = frame;
            this->particleManager->resetSystem((*this->trailSystems)[this->gun->fireIndex]);
            this->particleManager->enableSystemEmit((*this->trailSystems)[this->gun->fireIndex], true);
            this->particleManager->enableSystemRender((*this->trailSystems)[this->gun->fireIndex], true);
            (*this->trailTimers)[this->gun->fireIndex] = 0;
        }

        gun = this->gun;
        if (gun->weaponType == ITEM_SORT_CLUSTER_MISSILE && gun->fireIndex > 0) {
            // Cluster salvo: launch the remaining rockets in the same frame.
            gun->hitSmall = 1;
            gun->fireIndex = gun->fireIndex - 1;
            this->update(elapsed);
            this->gun->update(elapsed);
            gun = this->gun;
        }
    }

    if (gun->active != 0 && gun->count == 1 && ((const Vector *) gun->positions)[0].z == 50000.0f) {
        // Single rocket gone: stop its trail.
        gun->active = 0;
        const int kind = this->rocketKind;
        if ((uint32_t) (kind - 4) < 2 || kind == 0x28) {
            if (this->fadeTimer == 0)
                this->fadeTimer = 2000;
            this->radar->level->field_80->enableSystemEmit(this->particleSystem, false);
        } else {
            Level *level = this->radar->level;
            (gun->itemIndex == 0xe8 ? level->field_9c : level->particleRenderBoolPtr)
                    ->enableSystemEmit(this->particleSystem, false);
        }
    } else if (this->fadeTimer > 0 && gun->count == 1 && ((const Vector *) gun->positions)[0].z == 50000.0f) {
        const int left = this->fadeTimer - elapsed;
        this->fadeTimer = left;
        if (left <= 0) {
            this->fadeTimer = 0;
            this->particleManager->enableSystemRender(this->particleSystem, false);
        }
    }

    const float dt = (float) elapsed;
    for (uint32_t i = 0; i < this->gun->count; ++i) {
        gun = this->gun;
        ahead = ((const Vector *) gun->positions)[i] + ((const Vector *) gun->velocities)[i] * dt * 0.5f;

        gun = this->gun;
        if (gun->active != 0) {
            if (ahead.z == 50000.0f) {
                if (this->trailMatrices == nullptr)
                    goto fade;
                this->particleManager->enableSystemEmit((*this->trailSystems)[i], false);
                if ((*this->trailTimers)[i] == 0)
                    (*this->trailTimers)[i] = 2000;
            } else {
                if (this->homing != 0 &&
                    (this->trailMatrices != nullptr || gun->lifetimes[i] < gun->initialLifetime - 1000)) {
                    this->seekEnemy(gun->lifetimes[i], i);
                    gun = this->gun;
                }
                if (gun->weaponType == ITEM_SORT_CLUSTER_MISSILE) {
                    // Cluster rockets corkscrew around their flight path, phase-shifted per rocket.
                    const uint32_t total = (uint32_t) gun->initialLifetime;
                    const float phase = (float) ((total * i) / gun->count);
                    const float s = Sinf((phase + (float) (int) (total - gun->lifetimes[i])) * 0.003f);
                    const float c = Cosf((phase + (float) (this->gun->initialLifetime - this->gun->lifetimes[i])) * 0.003f);
                    const Vector cross = VectorCross(((const Vector *) this->gun->velocities)[i],
                                                     ((const Vector *) this->gun->upVectors)[i]);
                    const Vector sideways = VectorNormalize(cross);
                    ((Vector *) this->gun->positions)[i] += sideways * (s + s) * dt;
                    ((Vector *) this->gun->positions)[i] += ((const Vector *) this->gun->upVectors)[i] * (c + c) * dt;
                }
            }

            if (this->trailMatrices != nullptr) {
                gun = this->gun;
                ahead = ((const Vector *) gun->positions)[i] + ((const Vector *) gun->velocities)[i] * dt * 0.5f;
                MatrixSetTranslation((*this->trailMatrices)[i], ahead.x, ahead.y, ahead.z);
                const Vector heading = VectorNormalize(((const Vector *) this->gun->velocities)[i]);
                Matrix &trail = (*this->trailMatrices)[i];
                trail.m[2] = heading.x;
                trail.m[6] = heading.y;
                trail.m[10] = heading.z;
                MatrixSetTranslation(trail, ahead.x, ahead.y, ahead.z);
            }
        }

    fade:
        if (this->trailTimers != nullptr && ahead.z == 50000.0f && (*this->trailTimers)[i] > 0) {
            const int left = (*this->trailTimers)[i] - elapsed;
            (*this->trailTimers)[i] = left;
            if (left <= 0) {
                (*this->trailTimers)[i] = 0;
                this->particleManager->enableSystemRender((*this->trailSystems)[i], false);
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

