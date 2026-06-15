#include "gof2/game/weapons/BombGun.h"

#include "gof2/engine/render/AEGeometry.h"
#include "gof2/game/ship/Player.h"
#include "gof2/engine/math/Transform.h"
#include "gof2/engine/math/AEMath.h"
#include "gof2/game/world/LevelScript.h"
#include "gof2/game/core/PaintCanvasClass.h"
#include "gof2/game/ship/PlayerEgo.h"
#include "gof2/game/ship/TargetFollowCamera.h"
#include "gof2/engine/audio/FModSound.h"
#include "gof2/game/weapons/RocketGun.h"
#include "gof2/game/weapons/Gun.h"
#include "gof2/game/mission/Status.h"
#include "gof2/game/mission/Explosion.h"

using AbyssEngine::AEMath::operator+;
using AbyssEngine::AEMath::operator-;
using AbyssEngine::AEMath::operator*;

// Engine globals: the active scene canvas and the FMOD / game-state singletons.
extern "C" void *g_PaintCanvas;
extern "C" void *g_FMod_singleton;
extern void *g_mining_status;

static inline PaintCanvas *activeCanvas()
{
    return (PaintCanvas *)*(void **)g_PaintCanvas;
}

static inline FModSound *sound()
{
    return (FModSound *)*(void **)g_FMod_singleton;
}

static inline Status *gameStatus()
{
    return (Status *)g_mining_status;
}

// BombGun's destructor releases its detonation Explosion, then chains into the
// RocketGun base teardown.
BombGun::~BombGun()
{
    delete this->explosion;
    this->explosion = nullptr;
}

// A guided "bomb" projectile (nuke / EMP / mine). It chains to the RocketGun base,
// spawns the matching Explosion effect, then builds its scene geometry: either a
// pre-authored AEGeometry attached to the rocket transform (normal launch), or a
// fresh mesh-backed transform tree (the simple-mesh path). Two fixed offset vectors
// (the launch ejection vectors) and a trail geometry round out the setup.
BombGun::BombGun(Gun *gun, uint32_t meshId, int rocketArg, int bombType, bool simpleMesh,
                 Level *level)
    : RocketGun(rocketArg, gun, meshId, 0, 0, bombType, false, level)
{
    this->detonationPosition = Vector{0.0f, 0.0f, 0.0f};
    this->cameraTargetOffset = Vector{0.0f, 0.0f, 0.0f};

    // Pick the Explosion variant from the bomb type / weapon item.
    int explosionType;
    if (bombType == 6) {
        explosionType = 7;          // EMP burst
    } else if (bombType == 0x2a) {
        explosionType = 0xb;        // large nuke
    } else if (gun->itemIndex == 0xe8) {
        explosionType = 0xd;        // special weapon
    } else {
        explosionType = 0;          // default
    }

    this->explosion = new Explosion(explosionType);
    this->explosion->setWeaponIndex(gun->itemIndex);

    this->playerControlled = simpleMesh ? 1 : 0;
    this->detonationPending = 1;
    this->bombType = bombType;
    this->geometryTransformId = 0xffffffff;

    if (bombType == 0x2a) {
        this->explosion->setScaling(50000.0f);
    }

    PaintCanvas *canvas = activeCanvas();

    if (!simpleMesh) {
        // Normal launch: attach a pre-authored geometry, unless the weapon explicitly
        // opts out (item 0xe8 with weaponType 0x22).
        int item = gun->itemIndex;
        bool customWeapon = item != 0xe8;
        int sel = customWeapon ? gun->weaponType : item;
        if (customWeapon && sel != 0x22) {
            // Mesh id depends on the projectile model.
            uint16_t geomMesh = 0x395d;
            if (meshId == 0x395a) geomMesh = 0x395b;
            if (meshId == 0x3958) geomMesh = 0x3959;

            AEGeometry *geo = new AEGeometry(geomMesh, canvas, false);

            this->geometryTransformId = geo->transform;
            canvas->TransformAddChild(this->transformId, geo->transform);

            AbyssEngine::Transform *transform =
                (AbyssEngine::Transform *)canvas->TransformGetTransform(geo->transform);
            AbyssEngine::AnimationMode mode =
                (meshId == 0x395c) ? (AbyssEngine::AnimationMode)2 : (AbyssEngine::AnimationMode)1;
            transform->SetAnimationState(mode, nullptr);

            delete geo;
        }
    } else {
        // Simple-mesh path: build a transform tree from the bomb's own mesh plus a
        // trail geometry, then re-parent it under the rocket transform.
        canvas->TransformCreate(&this->meshTransformId);
        canvas->TransformAddMesh(this->meshTransformId, this->meshId, false);

        AbyssEngine::Transform *transform =
            (AbyssEngine::Transform *)canvas->TransformGetTransform(this->meshTransformId);
        transform->SetAnimationState((AbyssEngine::AnimationMode)1, nullptr);

        AEGeometry *geo = new AEGeometry((uint16_t)0x37d6, canvas, false);
        canvas->TransformAddChild(this->meshTransformId, geo->transform);
        canvas->TransformRemoveMesh(this->transformId, (void *)(uintptr_t)this->meshTransformId);
        canvas->TransformAddChild(this->transformId, this->meshTransformId);

        delete geo;
    }

    // Two fixed rocket-cam offsets: camera (0, 450, -1400) and target (0, 0, 1700).
    this->cameraOffset = Vector{0.0f, 450.0f, -1400.0f};
    this->cameraTargetOffset = Vector{0.0f, 0.0f, 1700.0f};

    this->player = nullptr;

    // Trail geometry attached to the active scene canvas.
    this->trailGeometry = new AEGeometry(canvas);
}

void BombGun::setPlayer(PlayerEgo *player)
{
    this->player = player;
}

void BombGun::render()
{
    RocketGun::render();
    if (this->gun->ignited != 0)
        this->explosion->render();
}

void BombGun::update(int elapsed)
{
    PlayerEgo *player = this->player;
    if (player == nullptr)
        return;

    Gun *gun = this->gun;
    if (gun->ignited == 0) {
        if (this->bombType == 0x2a) {
            this->detonationPosition = player->getPosition();
        } else {
            this->detonationPosition = *(const Vector *)gun->positions;
        }
    }

    gun = this->gun;
    bool steerable = this->playerControlled != 0;
    bool updateTransforms = false;

    if (!steerable) {
        updateTransforms = gun->active != 0;
    } else if (gun->active != 0) {
        if (gun->ignited != 0) {
            updateTransforms = true;
        } else {
            PaintCanvas *canvas = activeCanvas();

            if (gun->hitSmall != 0) {
                gun->hitSmall = 0;
                AbyssEngine::Transform *transform =
                    (AbyssEngine::Transform *)canvas->TransformGetTransform(this->transformId);
                transform->SetAnimationState((AbyssEngine::AnimationMode)3, nullptr);
                transform = (AbyssEngine::Transform *)canvas->TransformGetTransform(this->transformId);
                transform->SetAnimationState((AbyssEngine::AnimationMode)1, nullptr);
                // Snapshot the wrapped Player's pose matrix into the PlayerEgo body.
                // This is a raw cross-object copy the recovery could not bind to a
                // named PlayerEgo field, so it stays as an explicit memory move.
                *(Matrix *)((char *)player + 0x10) = *(const Matrix *)((char *)player->player + 4);
                sound()->play(0x45c, nullptr, nullptr, 0.0f);
            }

            AEGeometry *geometry = this->trailGeometry;
            geometry->setMatrix(*(const Matrix *)canvas->TransformGetLocal(this->transformId));
            Vector offset = 350.0f * *(const Vector *)gun->velocities;
            geometry->setPosition(*(const Vector *)gun->positions + offset);
            geometry->updateReferenceMatrix();

            TargetFollowCamera *camera =
                (TargetFollowCamera *)(intptr_t)player->getTargetFollowCamera();
            camera->setTarget(geometry);
            camera->setCamOffset(&this->cameraOffset);
            camera->setTargetOffset(&this->cameraTargetOffset);
            camera->useTargetsUpVector(false);
            player->setRocketControl(gun, geometry);

            if (*(int *)gun->lifetimes < gun->initialLifetime - 500) {
                AbyssEngine::Transform *transform =
                    (AbyssEngine::Transform *)canvas->TransformGetTransform(this->transformId);
                transform->Update((int64_t)elapsed, false);
            }

            float bank = player->getRocketBanking() * 0.2f;
            this->bankingAngle = bank;
            sound()->setParamValue(0, 0x45c, bank * 0.5f);
        }
    }

    if (updateTransforms) {
        PaintCanvas *canvas = activeCanvas();
        AbyssEngine::Transform *transform =
            (AbyssEngine::Transform *)canvas->TransformGetTransform(this->transformId);
        transform->Update((int64_t)elapsed, false);
        if (this->geometryTransformId != 0xffffffff) {
            transform = (AbyssEngine::Transform *)canvas->TransformGetTransform(this->geometryTransformId);
            transform->Update((int64_t)elapsed, false);
        }
    }

    RocketGun::update(elapsed);
    gun = this->gun;
    if (gun->ignited == 0)
        return;

    if (this->detonationPending != 0) {
        if (this->playerControlled != 0) {
            player = this->player;
            player->levelScript->resetCamera(player->level);
            player->setRocketControl(nullptr, this->trailGeometry);
            sound()->stop(0x45c);
        }

        this->rumbleTimer = 0;
        player = this->player;
        Vector toPlayer = this->detonationPosition - player->getPosition();
        float distance = AbyssEngine::AEMath::VectorLength(toPlayer);
        float magnitude = (float)this->gun->getMagnitude();
        float force = (((magnitude * 0.5f) - distance) / (magnitude * 0.5f)) * 0.5f;
        if (force > 1.0f)
            force = 1.0f;
        if (force < 0.0f)
            force = 0.0f;
        if (this->bombType == 0x2a)
            force *= 0.2f;
        if (gameStatus()->hardCoreMode() != 0) {
            ((Player *)player->player)->damage((int)(force * (float)this->gun->damage));
        }
        player->addNukeVolatileForce(force);

        float capped = (distance < 30000.0f) ? distance : 30000.0f;
        this->rumbleStrength = 1.0f - capped / 30000.0f;
        ((TargetFollowCamera *)(intptr_t)player->getTargetFollowCamera())
            ->setRumblePercentage(this->rumbleStrength, 0x32);

        Vector direction =
            (this->bombType == 0x2a) ? player->GetDirVector() : Vector{0.0f, 0.0f, 0.0f};
        this->explosion->start(&this->detonationPosition, &direction);
        this->detonationPending = 0;
    }

    this->explosion->update(elapsed, nullptr);
    int timer = this->rumbleTimer + elapsed;
    if (timer > 2000)
        timer = 2000;
    this->rumbleTimer = timer;

    player = this->player;
    float rumble = this->rumbleStrength * ((float)timer / -2000.0f + 1.0f);
    ((TargetFollowCamera *)(intptr_t)player->getTargetFollowCamera())
        ->setRumblePercentage(rumble, 0x32);

    if (this->explosion->isPlaying() == 0) {
        ((TargetFollowCamera *)(intptr_t)player->getTargetFollowCamera())
            ->setRumblePercentage(0.0f, 0);
        this->rumbleTimer = 0;
        this->gun->ignited = 0;
        this->detonationPending = 1;
        this->explosion->reset();
    }
}
