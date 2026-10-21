#include "game/weapons/BombGun.h"

#include "engine/render/AEGeometry.h"
#include "game/ship/Player.h"
#include "engine/math/Transform.h"
#include "engine/math/AEMath.h"
#include "game/world/LevelScript.h"
#include "engine/render/PaintCanvas.h"
#include "game/ship/PlayerEgo.h"
#include "game/ship/TargetFollowCamera.h"
#include "engine/audio/FModSound.h"
#include "game/weapons/RocketGun.h"
#include "game/weapons/Gun.h"
#include "game/mission/Status.h"
#include "game/mission/Explosion.h"

#include "game/core/Globals.h"

BombGun::BombGun(Gun *gun, uint32_t meshId, int rocketArg, int bombType, bool simpleMesh,
                 Level *level)
    : RocketGun(rocketArg, gun, meshId, 0, 0, bombType, false, level), cameraOffset(),
      cameraTargetOffset() {

    this->explosion = new Explosion(bombType == ITEM_SORT_EMP_BOMB       ? 7
                                    : bombType == ITEM_SORT_SHOCK_BLAST ? 0xb
                                    : gun->itemIndex == 0xe8            ? 0xd
                                                                        : 0);
    this->explosion->setWeaponIndex(gun->itemIndex);

    this->detonationPending = 1;
    this->useEgoOrientation = simpleMesh;
    this->bombType = bombType;
    this->geometryTransformId = 0xffffffff;
    if (bombType == ITEM_SORT_SHOCK_BLAST)
        this->explosion->setScaling(50000.0f);

    if (simpleMesh) {
        Globals::Canvas->TransformCreate(this->secondaryTransform);
        Globals::Canvas->TransformAddMesh(this->secondaryTransform, this->meshId, false);
        static_cast<AbyssEngine::Transform *>(Globals::Canvas->TransformGetTransform(this->secondaryTransform))->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);

        AEGeometry *geo = new AEGeometry((uint16_t) 0x37d6, Globals::Canvas, false);
        Globals::Canvas->TransformAddChild(this->secondaryTransform, geo->transform);
        Globals::Canvas->TransformRemoveMesh(this->transform, this->meshId);
        Globals::Canvas->TransformAddChild(this->transform, this->secondaryTransform);
        delete geo;
    } else if (gun->itemIndex != 0xe8 && gun->weaponType != ITEM_SORT_IONIZING_MISSILE) {
        uint16_t geomMesh = 0x395d;
        if (meshId == 0x395a)
            geomMesh = 0x395b;
        if (meshId == 0x3958)
            geomMesh = 0x3959;
        AEGeometry *geo = new AEGeometry(geomMesh, Globals::Canvas, false);
        this->geometryTransformId = geo->transform;
        Globals::Canvas->TransformAddChild(this->transform, geo->transform);
        if (meshId == 0x395c)
            static_cast<AbyssEngine::Transform *>(Globals::Canvas->TransformGetTransform(this->geometryTransformId))->SetAnimationState((AbyssEngine::AnimationMode) 2, nullptr);
        else
            static_cast<AbyssEngine::Transform *>(Globals::Canvas->TransformGetTransform(this->transform))->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
        delete geo;
    }

    this->cameraOffset = Vector{0.0f, 450.0f, -1400.0f};
    this->cameraTargetOffset = Vector{0.0f, 0.0f, 1700.0f};
    this->player = nullptr;
    this->trailGeometry = new AEGeometry(Globals::Canvas);
}

BombGun::~BombGun() {
    delete this->explosion;
    this->explosion = nullptr;
}

void BombGun::setPlayer(PlayerEgo *player) {
    this->player = player;
}

void BombGun::update(int elapsed) {
    PlayerEgo *player = this->player;
    if (player == nullptr)
        return;

    if (this->gun->ignited == 0) {
        this->detonationPosition = (this->bombType == ITEM_SORT_SHOCK_BLAST)
                                           ? player->getPosition()
                                           : *this->gun->positions.data();
    }

    Gun *gun = this->gun;
    if (this->useEgoOrientation != 0) {
        if (gun->active != 0) {
            if (gun->ignited != 0) {
                goto updateTransforms;
            }
            if (gun->hitSmall != 0) {
                gun->hitSmall = 0;
                static_cast<AbyssEngine::Transform *>(Globals::Canvas->TransformGetTransform(this->transform))->SetAnimationState((AbyssEngine::AnimationMode) 3, nullptr);
                static_cast<AbyssEngine::Transform *>(Globals::Canvas->TransformGetTransform(this->transform))->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
                this->player->field_0x40 =
                        reinterpret_cast<AbyssEngine::AEMath::Matrix &>(this->player->player->transform[0]);
                Globals::sound->play(0x45c, nullptr, nullptr, 0.0f);
            }

            AEGeometry *geometry = this->trailGeometry;
            geometry->setMatrix(*(const Matrix *) Globals::Canvas->TransformGetLocal(this->transform));
            this->trailGeometry->setPosition(*this->gun->positions.data() +
                                             350.0f * *this->gun->velocities.data());
            this->trailGeometry->updateReferenceMatrix();

            this->player->getTargetFollowCamera()->setTarget(this->trailGeometry);
            this->player->getTargetFollowCamera()->setCamOffset(this->cameraOffset);
            this->player->getTargetFollowCamera()->setTargetOffset(this->cameraTargetOffset);
            this->player->getTargetFollowCamera()->useTargetsUpVector(false);
            this->player->setRocketControl(this->gun, this->trailGeometry);

            if (*this->gun->lifetimes < this->gun->initialLifetime - 500)
                static_cast<AbyssEngine::Transform *>(Globals::Canvas->TransformGetTransform(this->transform))->Update((int64_t) elapsed, false);

            this->rollAngle = this->player->getRocketBanking() * 0.2f;
            Globals::sound->setParamValue(0, 0x45c, this->rollAngle * 0.5f);
        }
    } else if (gun->active != 0) {
    updateTransforms:
        static_cast<AbyssEngine::Transform *>(Globals::Canvas->TransformGetTransform(this->transform))->Update((int64_t) elapsed, false);
        if (this->geometryTransformId != 0xffffffff)
            static_cast<AbyssEngine::Transform *>(Globals::Canvas->TransformGetTransform(this->geometryTransformId))->Update((int64_t) elapsed, false);
    }

    RocketGun::update(elapsed);
    if (this->gun->ignited == 0)
        return;

    if (this->detonationPending != 0) {
        if (this->useEgoOrientation != 0) {
            this->player->levelScript->resetCamera(this->player->level);
            this->player->setRocketControl(nullptr, this->trailGeometry);
            Globals::sound->stop(0x45c);
        }

        this->rumbleTimer = 0;
        float distance = AbyssEngine::AEMath::VectorLength(this->detonationPosition - this->player->getPosition());
        float halfRange = (float) this->gun->getMagnitude() * 0.5f;
        float force = ((halfRange - distance) / ((float) this->gun->getMagnitude() * 0.5f)) * 0.5f;
        if (force > 1.0f)
            force = 1.0f;
        else if (force < 0.0f)
            force = 0.0f;
        if (this->bombType == ITEM_SORT_SHOCK_BLAST)
            force *= 0.2f;
        if (Globals::status->hardCoreMode())
            this->player->player->damage((int) (force * (float) this->gun->damage), false, -1);
        this->player->addNukeVolatileForce(force);

        float capped = (distance < 30000.0f) ? distance : 30000.0f;
        this->rumbleStrength = 1.0f - capped / 30000.0f;
        this->player->getTargetFollowCamera()->setRumblePercentage(this->rumbleStrength, 0x32);

        if (this->bombType == ITEM_SORT_SHOCK_BLAST)
            this->explosion->start(this->detonationPosition, this->player->GetDirVector());
        else
            this->explosion->start(this->detonationPosition, Vector{0.0f, 0.0f, 0.0f});
        this->detonationPending = 0;
    }

    this->explosion->update(elapsed, static_cast<TargetFollowCamera *>(nullptr));
    int timer = this->rumbleTimer + elapsed;
    if (timer > 2000)
        timer = 2000;
    this->rumbleTimer = timer;
    this->player->getTargetFollowCamera()->setRumblePercentage(
            this->rumbleStrength * ((float) this->rumbleTimer / -2000.0f + 1.0f), 0x32);

    if (!this->explosion->isPlaying()) {
        this->player->getTargetFollowCamera()->setRumblePercentage(0.0f, 0);
        this->rumbleTimer = 0;
        this->gun->ignited = 0;
        this->detonationPending = 1;
        this->explosion->reset();
    }
}

void BombGun::render() {
    RocketGun::render();
    if (this->gun->ignited != 0)
        this->explosion->render();
}

