
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

// The four missile trail sets. setRadar reads the constexpr copy so its switch folds to the
// original constants; the runtime copy is kept in .data so the page-quantized GOT stays aligned
// with the 1896-byte update body (the libstdc++ exception ctors encode a GOT displacement).
static constexpr ParticleSettings::ParticleSet kRocketTrailSetValues[4] = {
        ParticleSettings::ParticleSet_0x19,
        ParticleSettings::ParticleSet_0x1a,
        ParticleSettings::ParticleSet_0x1b,
        ParticleSettings::ParticleSet_0x1c,
};
[[gnu::used]] static ParticleSettings::ParticleSet kRocketTrailSets[4] = {
        kRocketTrailSetValues[0],
        kRocketTrailSetValues[1],
        kRocketTrailSetValues[2],
        kRocketTrailSetValues[3],
};
static_assert(sizeof(kRocketTrailSets) == 16, "rocket trail-set table size");

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
    bool special;
    switch (item) {
        case 0x1c:
        case 0x1d:
        case 0x1e:
        case 0xc1:
            special = true;
            break;
        default:
            special = this->gun->weaponType == ITEM_SORT_CLUSTER_MISSILE;
            break;
    }
    if (special) {
        int trailMode = item - 0x1c; // 0..2: the three missile tiers
        if (this->gun->weaponType == ITEM_SORT_CLUSTER_MISSILE)
            trailMode = 0;
        else if (item == 0xc1)
            trailMode = 3;

        this->trailMatrices = new Array<Matrix>();
        this->trailSystems = new Array<int>();
        this->trailTimers = new Array<int>();
        ArraySetLength(this->gun->positions.count, *this->trailMatrices);
        ArraySetLength(this->gun->positions.count, *this->trailSystems);
        ArraySetLength(this->gun->positions.count, *this->trailTimers);

        ParticleSettings::ParticleSet defaultSet =
                trailMode == 2 ? kRocketTrailSetValues[2] : kRocketTrailSetValues[3];
        for (uint32_t i = 0; i < this->gun->positions.count; ++i) {
            ParticleSystemManager *manager;
            if (this->gun->itemIndex == 0xc1) {
                this->particleManager = radar->level->field_98;
                manager = this->particleManager;
            } else {
                manager = this->particleManager;
            }
            Matrix *trail = &(*this->trailMatrices)[i];
            ParticleSettings::ParticleSet set = defaultSet;
            switch (trailMode) {
                case 1: set = kRocketTrailSetValues[1]; break;
                case 0: set = kRocketTrailSetValues[0]; break;
            }
            (*this->trailSystems)[i] = manager->addSystem(trail, set, false);
            this->particleManager->enableSystemEmit((*this->trailSystems)[i], false);
            (*this->trailTimers)[i] = 0;
        }
        return;
    }

    if ((unsigned int) (this->rocketKind - 4) < 2 || this->rocketKind == 0x28) {
        this->trailMatrices = new Array<Matrix>();
        this->trailSystems = new Array<int>();
        this->trailTimers = new Array<int>();
        ArraySetLength(this->gun->positions.count, *this->trailMatrices);
        ArraySetLength(this->gun->positions.count, *this->trailSystems);
        ArraySetLength(this->gun->positions.count, *this->trailTimers);
        for (uint32_t i = 0; i < this->gun->positions.count; ++i) {
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
    const float dt = (float) elapsed;
    Vector ahead = {0.0f, 0.0f, 0.0f};
    Gun &g = *this->gun;
    g.update(elapsed);

    if (this->hasGeometry != 0 && this->gun->delayActive != 0) {
        // Muzzle flash grows on the launcher while the launch delay runs down.
        PlayerEgo *player = this->level->getPlayer();
        Vector muzzle = player->getPosition();
        Vector barrel = {g.offset.x, g.offset.y, g.offset.z - 100.0f};
        const Matrix &ship = reinterpret_cast<const Matrix &>(player->player->transform[0]);
        muzzle += MatrixRotateVector(ship, barrel);
        this->geometry->setPosition(muzzle);
        const float scale = (float) (int) (((float) this->gun->delayTimer / -200.0f + 1.0f) * 255.0f) / 255.0f;
        this->geometry->setScaling(scale, scale, scale);
        const Vector forward = MatrixGetDir(reinterpret_cast<const Matrix &>(player->player->transform[0]));
        Vector up = {0.0f, 1.0f, 0.0f};
        this->geometry->setDirection(forward, up);
    }

    gun = this->gun;
    this->wasFiring = gun->delayActive;

    if (gun->hitSmall != 0) {
        // A rocket was just launched: orient the rocket mesh along its velocity.
        this->fadeTimer = 0;
        this->activeShotId = -1;
        gun->hitSmall = 0;

        PaintCanvas *canvas = Globals::Canvas;
        Matrix &local = *(Matrix *) canvas->TransformGetLocal(this->transform);
        const Vector &shot = this->gun->positions.data()[this->gun->currentProjectile];
        MatrixSetTranslation(local, shot.x, shot.y, shot.z);
        Vector heading = this->gun->velocities.data()[this->gun->currentProjectile];

        Matrix frame = *(const Matrix *) Globals::Canvas->TransformGetLocal(this->transform);
        Vector up = {0.0f, 1.0f, 0.0f};
        Vector side = VectorCross(up, heading);
        side = VectorNormalize(side);
        up = VectorCross(heading, side);
        up = VectorNormalize(up);
        frame.m.e0 = side.x;
        frame.m.e4 = side.y;
        frame.m.e8 = side.z;
        frame.m.e1 = up.x;
        frame.m.e5 = up.y;
        frame.m.e9 = up.z;
        frame.m.e2 = heading.x;
        frame.m.e6 = heading.y;
        frame.m.e10 = heading.z;
        Globals::Canvas->TransformSetLocal(this->transform, frame);

        if (this->trailMatrices == nullptr) {
            const int kind = this->rocketKind;
            if ((uint32_t) (kind - 4) < 2 || kind == 0x28) {
                this->radar->level->field_80->enableSystemRender(this->particleSystem, true);
                this->radar->level->field_80->enableSystemEmit(this->particleSystem, true);
                goto launchDone;
            }
            goto itemEmit;
        } else if (this->gun->positions.data()[this->gun->currentProjectile].z == 50000.0f) {
            this->particleManager->enableSystemEmit((*this->trailSystems)[this->gun->currentProjectile], false);
            this->particleManager->resetSystem((*this->trailSystems)[this->gun->currentProjectile]);
            (*this->trailTimers)[this->gun->currentProjectile] = 0;
            goto launchDone;
        } else {
            (*this->trailMatrices)[this->gun->currentProjectile] = frame;
            this->particleManager->resetSystem((*this->trailSystems)[this->gun->currentProjectile]);
            this->particleManager->enableSystemEmit((*this->trailSystems)[this->gun->currentProjectile], true);
            this->particleManager->enableSystemRender((*this->trailSystems)[this->gun->currentProjectile], true);
            (*this->trailTimers)[this->gun->currentProjectile] = 0;
            goto launchDone;
        }

    itemEmit:
        if (this->gun->itemIndex == 0xe8) {
            this->radar->level->field_9c->enableSystemEmit(this->particleSystem, true);
        } else {
            this->radar->level->particleRenderBoolPtr->enableSystemEmit(this->particleSystem,
                                                                       this->gun->itemIndex != 0xb3);
        }

    launchDone:
        if (gun->weaponType == ITEM_SORT_CLUSTER_MISSILE && gun->currentProjectile > 0) {
            // Cluster salvo: launch the remaining rockets in the same frame.
            gun->currentProjectile = gun->currentProjectile - 1;
            gun->hitSmall = 1;
            this->update(elapsed);
            this->gun->update(elapsed);
            gun = this->gun;
        }
    }

    if (gun->active != 0 && gun->positions.count == 1 && gun->positions.data()[0].z == 50000.0f) {
        // Single rocket gone: stop its trail.
        gun->active = 0;
        const int kind = this->rocketKind;
        if ((uint32_t) (kind - 4) < 2 || kind == 0x28) {
            if (this->fadeTimer == 0)
                this->fadeTimer = 2000;
            this->radar->level->field_80->enableSystemEmit(this->particleSystem, false);
        } else {
            Level *level = this->radar->level;
            if (gun->itemIndex == 0xe8)
                level->field_9c->enableSystemEmit(this->particleSystem, false);
            else
                level->particleRenderBoolPtr->enableSystemEmit(this->particleSystem, false);
        }
    } else if (this->fadeTimer > 0 && gun->positions.count == 1 && gun->positions.data()[0].z == 50000.0f) {
        const int left = this->fadeTimer - elapsed;
        this->fadeTimer = left;
        if (left <= 0) {
            this->fadeTimer = 0;
            this->particleManager->enableSystemRender(this->particleSystem, false);
        }
    }

    Array<Matrix> *trailMatrices = this->trailMatrices;
    for (uint32_t i = 0; i < this->gun->positions.count; ++i) {
        gun = this->gun;
        ahead = gun->positions.data()[i] + gun->velocities.data()[i] * dt * 0.5f;

        gun = this->gun;
        if (gun->active != 0) {
            if (ahead.z == 50000.0f) {
                if (trailMatrices == nullptr)
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
                    const float phase = (float) ((total * i) / gun->positions.count);
                    const float s = Sinf((phase + (float) (int) (total - gun->lifetimes[i])) * 0.003f);
                    const float c = Cosf((phase + (float) (this->gun->initialLifetime - this->gun->lifetimes[i])) * 0.003f);
                    const Vector cross = VectorCross(this->gun->velocities.data()[i],
                                                     this->gun->upVectors.data()[i]);
                    this->gun->positions.data()[i] += VectorNormalize(cross) * (s + s) * dt;
                    this->gun->positions.data()[i] += this->gun->upVectors.data()[i] * (c + c) * dt;
                }
            }

            if (trailMatrices != nullptr) {
                gun = this->gun;
                ahead = gun->positions.data()[i] + gun->velocities.data()[i] * dt * 0.5f;
                MatrixSetTranslation((*trailMatrices)[i], ahead.x, ahead.y, ahead.z);
                const Vector heading = VectorNormalize(this->gun->velocities.data()[i]);
                Matrix &trail = (*trailMatrices)[i];
                trail.m.e2 = heading.x;
                trail.m.e6 = heading.y;
                trail.m.e10 = heading.z;
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
    steer = AbyssEngine::AEMath::VectorNormalize(enemy->getPosition() - this->gun->positions.data()[index]);
    Vector heading = AbyssEngine::AEMath::VectorNormalize(this->gun->velocities.data()[index]);
    steer -= heading;
    steer /= this->turnRate * 20.0f;
    heading += steer;
    this->gun->velocities.data()[index] = AbyssEngine::AEMath::VectorNormalize(heading);
    this->gun->velocities.data()[index] *= this->gun->pitchRate;
}

void RocketGun::translate(const Vector & /*v*/) {
}

