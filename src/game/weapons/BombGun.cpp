#include "gof2/game/weapons/BombGun.h"
#include <new>
#include "gof2/game/ship/TargetFollowCamera.h"
#include "gof2/engine/audio/FModSound.h"
#include "gof2/game/world/LevelScript.h"
#include "gof2/game/weapons/RocketGun.h"
#include "gof2/game/mission/Status.h"
#include "gof2/engine/math/Transform.h"
#include "gof2/game/mission/Explosion.h"
#include "gof2/game/weapons/Gun.h"
#include "gof2/engine/render/AEGeometry.h"

namespace AbyssEngine { namespace AEMath {
    Vector operator+(const Vector &, const Vector &);
    Vector operator-(const Vector &, const Vector &);
    Vector operator*(float, const Vector &);
} }

// NOTE: gof2/Player.h and gof2/PlayerEgo.h are intentionally NOT included. Those
// (out-of-batch) headers have pre-existing self-collisions that break compilation
// (Player.h: method 'turnedEnemy()' vs. field 'turnedEnemy'; PlayerEgo.h: redefines
// the byte-accessor helper 'I' already provided by common.h). BombGun only needs a
// few members/methods of each, so minimal local layouts are provided here. The
// 'getTargetFollowCamera()' return type is the true TargetFollowCamera* (PlayerEgo.h
// mistypes it as int).
struct Player {
    void damage(int amount);
};

struct PlayerEgo {
    void*        field_0x0;             // +0x0  (holds a Player*)
    AEGeometry*  field_0x4;             // +0x4
    AEGeometry*  field_0x8;             // +0x8
    Level*       field_0xc;             // +0xc
    LevelScript* field_0x10;           // +0x10

    void getPosition();
    void GetDirVector();
    void setRocketControl(void* gun, void* geo);
    int  getRocketBanking();
    void addNukeVolatileForce(float v);
    TargetFollowCamera* getTargetFollowCamera();
};

extern "C" __attribute__((visibility("hidden"))) void *BombGun_vtable;
extern "C" void *Explosion_dtor(Explosion *self);
extern "C" void *BombGun_base_dtor(BombGun *self);
void *_ZN7BombGunD1Ev(BombGun *self);
extern "C" __attribute__((visibility("hidden"))) void **BombGun_update_canvas_a;
extern "C" __attribute__((visibility("hidden"))) void **BombGun_update_canvas_b;
extern "C" __attribute__((visibility("hidden"))) void **BombGun_sound_play;
extern "C" __attribute__((visibility("hidden"))) void **BombGun_sound_param;
extern "C" __attribute__((visibility("hidden"))) void **BombGun_status;
extern "C" __attribute__((visibility("hidden"))) void **BombGun_sound_stop;
namespace AbyssEngine { namespace PaintCanvas {
AbyssEngine::Transform *TransformGetTransform(void *canvas, uint32_t transform);
void *TransformGetLocal(void *canvas, uint32_t transform);
void TransformCreate(void *canvas, uint32_t *out);
void TransformAddChild(void *canvas, uint32_t parent, uint32_t child);
void TransformRemoveMesh(void *canvas, uint32_t transform, uint16_t mesh);
void TransformAddMesh(void *canvas, uint32_t transform, uint16_t mesh, int flags);
} }
float VectorLength(void *self);
extern "C" __attribute__((visibility("hidden"))) void **BombGun_player_canvas;
extern "C" __attribute__((visibility("hidden"))) void **BombGun_canvas;
extern "C" __attribute__((visibility("hidden"))) void **BombGun_final_canvas;
extern "C" void Explosion_ctor(Explosion *self, int type);

void *_ZN7BombGunD1Ev(BombGun *self)
{
    *(void **)self = (char *)BombGun_vtable + 8;

    Explosion *explosion = self->explosion;
    if (explosion != 0)
        ::operator delete(Explosion_dtor(explosion));

    self->explosion = 0;
    return BombGun_base_dtor(self);
}

void _ZN7BombGunD0Ev(BombGun *self)
{
    return ::operator delete(_ZN7BombGunD1Ev(self));
}

// BombGun's destructor chains into the RocketGun base destructor once its own
// Explosion member has been released. base_dtor is exactly that base sub-object
// teardown: forward to RocketGun::~RocketGun (the non-deleting D1) and hand the
// object back so the caller can free the storage.
void *_ZN9RocketGunD1Ev(RocketGun *self);
extern "C" void *BombGun_base_dtor(BombGun *self)
{
    return _ZN9RocketGunD1Ev((RocketGun *)self);
}

// PaintCanvas singleton holder used by the ctor to register the bomb's geometry/
// transform with the active scene (same engine-global the update/render paths use).
extern "C" __attribute__((visibility("hidden"))) void **BombGun_ctor_canvas;

// Explosion::setWeaponIndex actually returns the per-weapon visual scaling factor
// (the engine prototype types it void). Reach the value without touching Explosion.h.
typedef float (*ExplosionSetWeaponIndexFn)(Explosion *, int);

// BombGun::BombGun(Gun*, unsigned int meshId, int rocketArg, int bombType, bool simpleMesh, Level*)
//
// A guided "bomb" projectile (nuke / EMP / mine). It chains to the RocketGun base,
// spawns the matching Explosion effect, then builds its scene geometry: either a
// pre-authored AEGeometry attached to the rocket transform (normal launch), or a
// fresh mesh-backed transform tree (the simple-mesh path). Two fixed offset vectors
// (the launch ejection vectors) and a trail geometry round out the setup.
BombGun::BombGun(Gun *gun, uint32_t meshId, int rocketArg, int bombType, bool simpleMesh,
                 Level *level)
{
    new ((RocketGun *)this) RocketGun(rocketArg, gun, meshId, 0, 0, bombType, false, level);

    // vtable + scalar state
    *(void **)this = (char *)BombGun_vtable + 8;
    this->detonationPosition = (Vector){0.0f, 0.0f, 0.0f};
    this->cameraTargetOffset = (Vector){0.0f, 0.0f, 0.0f};

    // Pick the Explosion variant from the bomb type / weapon item.
    int explosionType;
    if (bombType == 6) {
        explosionType = 7;          // EMP burst
    } else if (bombType == 0x2a) {
        explosionType = 0xb;        // large nuke
    } else if (((Gun *)gun)->itemIndex == 0xe8) {
        explosionType = 0xd;        // special weapon
    } else {
        explosionType = 0;          // default
    }

    Explosion *explosion = (Explosion *)operator new(0x68);
    new (explosion) Explosion(explosionType);
    this->explosion = explosion;

    // setWeaponIndex assigns the sound id and returns the per-weapon visual scale; the
    // shipped prototype types it void, so reinterpret its address to recover the value.
    void (Explosion::*swiMember)(int) = &Explosion::setWeaponIndex;
    ExplosionSetWeaponIndexFn swiFn;
    __builtin_memcpy(&swiFn, &swiMember, sizeof(swiFn));
    float weaponScale = swiFn(explosion, ((Gun *)gun)->itemIndex);

    this->playerControlled = simpleMesh ? 1 : 0;
    this->detonationPending = 1;
    this->bombType = bombType;
    this->geometryTransformId = 0xffffffff;

    if (bombType == 0x2a) {
        ((Explosion *)explosion)->setScaling(weaponScale);
    }

    void *canvas = *BombGun_ctor_canvas;

    if (!simpleMesh) {
        // Normal launch: attach a pre-authored geometry, unless the weapon explicitly
        // opts out (item 0xe8 with weaponType 0x22).
        int item = ((Gun *)gun)->itemIndex;
        bool customWeapon = item != 0xe8;
        int sel = customWeapon ? ((Gun *)gun)->weaponType : item;
        if (!(customWeapon == false || sel == 0x22)) {
            // Mesh id depends on the projectile model.
            uint16_t geomMesh = 0x395d;
            if (meshId == 0x395a) geomMesh = 0x395b;
            if (meshId == 0x3958) geomMesh = 0x3959;

            AEGeometry *geo = (AEGeometry *)operator new(0xc0);
            new (geo) AEGeometry(geomMesh, (PaintCanvas *)canvas, false);

            this->geometryTransformId = geo->transform;
            AbyssEngine::PaintCanvas::TransformAddChild(canvas, this->transformId, geo->transform);

            AbyssEngine::Transform *transform =
                AbyssEngine::PaintCanvas::TransformGetTransform(canvas, geo->transform);
            AbyssEngine::AnimationMode mode =
                (meshId == 0x395c) ? (AbyssEngine::AnimationMode)2 : (AbyssEngine::AnimationMode)1;
            ((AbyssEngine::Transform *)transform)->SetAnimationState(mode, 0);

            ((AEGeometry *)geo)->~AEGeometry();
            operator delete(geo);
        }
    } else {
        // Simple-mesh path: build a transform tree from the bomb's own mesh plus a
        // trail geometry, then re-parent it under the rocket transform.
        AbyssEngine::PaintCanvas::TransformCreate(canvas, &this->meshTransformId);
        AbyssEngine::PaintCanvas::TransformAddMesh(canvas, this->meshTransformId, this->meshId, 0);

        AbyssEngine::Transform *transform =
            AbyssEngine::PaintCanvas::TransformGetTransform(canvas, this->meshTransformId);
        ((AbyssEngine::Transform *)transform)->SetAnimationState((AbyssEngine::AnimationMode)1, 0);

        AEGeometry *geo = (AEGeometry *)operator new(0xc0);
        new (geo) AEGeometry((uint16_t)0x37d6, (PaintCanvas *)canvas, false);
        AbyssEngine::PaintCanvas::TransformAddChild(canvas, this->meshTransformId, geo->transform);
        AbyssEngine::PaintCanvas::TransformRemoveMesh(canvas, this->transformId,
                                                      (uint16_t)this->meshTransformId);
        AbyssEngine::PaintCanvas::TransformAddChild(canvas, this->transformId, this->meshTransformId);

        ((AEGeometry *)geo)->~AEGeometry();
        operator delete(geo);
    }

    // Two fixed rocket-cam offsets: camera (0, 450, -1400) and target (0, 0, 1700).
    this->cameraOffset = (Vector){0.0f, 450.0f, -1400.0f};
    this->cameraTargetOffset = (Vector){0.0f, 0.0f, 1700.0f};

    this->player = 0;

    // Trail geometry attached to the active scene canvas.
    AEGeometry *trail = (AEGeometry *)operator new(0xc0);
    new (trail) AEGeometry((PaintCanvas *)canvas);
    this->trailGeometry = trail;
}

void BombGun::setPlayer(PlayerEgo *player)
{
    this->player = player;
}

void BombGun::render()
{
    ((RocketGun *)(this))->render();
    if (this->gun->field_0x88 != 0)
        return ((Explosion *)(this->explosion))->render();
}

extern "C" __attribute__((visibility("hidden"))) TargetFollowCamera *(*BombGun_getCamera)(
    PlayerEgo *player);

// The bomb's rocket-cam steering reads back the player's follow camera through an
// indirect call; the slot resolves to PlayerEgo::getTargetFollowCamera. Provide a
// thin forwarder and point the function-pointer global at it.
static TargetFollowCamera *BombGun_getCamera_impl(PlayerEgo *player)
{
    return player->getTargetFollowCamera();
}
extern "C" __attribute__((visibility("hidden")))
    TargetFollowCamera *(*BombGun_getCamera)(PlayerEgo *) = &BombGun_getCamera_impl;

static const float kRocketOffsetScale = 350.0f;
static const float kBombScale = 0.2f;
static const float kZero = 0.0f;
static const float kRumbleDistance = 30000.0f;
static const float kRumbleTime = -2000.0f;

void BombGun::update(int elapsed)
{
    char scaled[12];
    char position[12];
    char work[12];

    PlayerEgo *player = this->player;
    if (player == 0)
        return;

    Gun *gun = this->gun;
    if (gun->field_0x88 == 0) {
        if (this->bombType == 0x2a) {
            ((PlayerEgo *)(position))->getPosition();
        } else {
            void *gunPos = gun->positions;
            *(uint64_t *)position = *(uint64_t *)gunPos;
            *(uint32_t *)(position + 8) = *(uint32_t *)((char *)gunPos + 0x8);
        }
        this->detonationPosition = *(const Vector *)(position);
    }

    gun = this->gun;
    bool playerControlled = this->playerControlled != 0;
    if (!playerControlled) {
        if (gun->field_0x4c != 0)
            goto update_transforms;
    } else if (gun->field_0x4c != 0) {
        if (gun->field_0x88 != 0)
            goto update_transforms;
        {
            void **canvas = BombGun_update_canvas_b;
            uint32_t *activeTransform = &this->transformId;
            if (gun->field_0x4d != 0) {
                gun->field_0x4d = 0;
                AbyssEngine::Transform *transform = AbyssEngine::PaintCanvas::TransformGetTransform(*canvas, *activeTransform);
                ((AbyssEngine::Transform *)(transform))->SetAnimationState((AbyssEngine::AnimationMode)3, 0);
                transform = AbyssEngine::PaintCanvas::TransformGetTransform(*canvas, *activeTransform);
                ((AbyssEngine::Transform *)(transform))->SetAnimationState((AbyssEngine::AnimationMode)1, 0);
                *(Matrix *)((char *)player + 0x10) = *(const Matrix *)((char *)player->field_0x0 + 4);
                ((FModSound *)(*BombGun_sound_play))->play(0x45c, 0, 0, 0.0f);
            }

            AEGeometry *geometry = this->trailGeometry;
            void *local = AbyssEngine::PaintCanvas::TransformGetLocal(*canvas, *activeTransform);
            ((AEGeometry *)geometry)->setMatrix(*(const AbyssEngine::AEMath::Matrix *)(local));
            *(Vector *)(scaled) = (kRocketOffsetScale) * *(const Vector *)(gun->velocities);
            *(Vector *)(position) = *(const Vector *)(gun->positions) + *(const Vector *)(scaled);
            ((AEGeometry *)(geometry))->setPosition(*(Vector *)position);
            ((AEGeometry *)(geometry))->updateReferenceMatrix();

            TargetFollowCamera *camera = BombGun_getCamera(player);
            ((TargetFollowCamera *)(camera))->setTarget(geometry);
            camera = BombGun_getCamera(player);
            ((TargetFollowCamera *)(camera))->setCamOffset(&this->cameraOffset);
            camera = BombGun_getCamera(player);
            ((TargetFollowCamera *)(camera))->setTargetOffset(&this->cameraTargetOffset);
            camera = BombGun_getCamera(player);
            ((TargetFollowCamera *)(camera))->useTargetsUpVector(false);
            ((PlayerEgo *)(player))->setRocketControl(gun, geometry);

            if (*(int *)gun->lifetimes < gun->initialLifetime - 500) {
                AbyssEngine::Transform *transform = AbyssEngine::PaintCanvas::TransformGetTransform(*canvas, *activeTransform);
                ((AbyssEngine::Transform *)(transform))->Update((int64_t)elapsed, 0);
            }

            float bank = ((PlayerEgo *)(player))->getRocketBanking() * kBombScale;
            this->bankingAngle = bank;
            ((FModSound *)(*BombGun_sound_param))->setParamValue(0, 0x45c, bank * 0.5f);
        }
    }
    goto after_transforms;

update_transforms:
    {
        void **canvas = BombGun_update_canvas_a;
        AbyssEngine::Transform *transform = AbyssEngine::PaintCanvas::TransformGetTransform(*canvas, this->transformId);
        ((AbyssEngine::Transform *)(transform))->Update((int64_t)elapsed, 0);
        if (this->geometryTransformId != -1) {
            transform = AbyssEngine::PaintCanvas::TransformGetTransform(*canvas, this->geometryTransformId);
            ((AbyssEngine::Transform *)(transform))->Update((int64_t)elapsed, 0);
        }
    }

after_transforms:

    ((RocketGun *)(this))->update(elapsed);
    gun = this->gun;
    if (gun->field_0x88 == 0)
        return;

    if (this->detonationPending != 0) {
        if (this->playerControlled != 0) {
            player = this->player;
            ((LevelScript *)(player->field_0x10))->resetCamera(player->field_0xc);
            ((PlayerEgo *)(player))->setRocketControl(0, this->trailGeometry);
            ((FModSound *)(*BombGun_sound_stop))->stop(0x45c);
        }

        this->rumbleTimer = 0;
        player = this->player;
        ((PlayerEgo *)(work))->getPosition();
        *(Vector *)(position) = this->detonationPosition - *(const Vector *)(work);
        float distance = VectorLength(position);
        float magnitude = (float)((Gun *)(this->gun))->getMagnitude();
        float force = (((magnitude * 0.5f) - distance) / (magnitude * 0.5f)) * 0.5f;
        if (force > 1.0f)
            force = 1.0f;
        if (force < kZero)
            force = kZero;
        if (this->bombType == 0x2a)
            force *= kBombScale;
        if (((Status *)(*BombGun_status))->hardCoreMode() != 0) {
            Gun *damageGun = this->gun;
            ((Player *)((Player *)player->field_0x0))->damage((int)(force * (float)damageGun->field_0x60));
        }
        ((PlayerEgo *)(player))->addNukeVolatileForce(force);

        float capped = kRumbleDistance;
        if (distance < kRumbleDistance)
            capped = distance;
        this->rumbleStrength = 1.0f - capped / kRumbleDistance;
        ((TargetFollowCamera *)(((PlayerEgo *)(player))->getTargetFollowCamera()))->setRumblePercentage(this->rumbleStrength, 0x32);

        Explosion *explosion = this->explosion;
        if (this->bombType == 0x2a) {
            ((PlayerEgo *)(position))->GetDirVector();
        } else {
            *(uint64_t *)position = 0;
            *(uint32_t *)(position + 8) = 0;
        }
        ((Explosion *)(explosion))->start(&this->detonationPosition,
                                          (const Vector *)position);
        this->detonationPending = 0;
    }

    ((Explosion *)(this->explosion))->update(elapsed, 0);
    int timer = this->rumbleTimer + elapsed;
    if (timer > 2000)
        timer = 2000;
    this->rumbleTimer = timer;

    player = this->player;
    float rumble = this->rumbleStrength * ((float)timer / kRumbleTime + 1.0f);
    ((TargetFollowCamera *)(((PlayerEgo *)(player))->getTargetFollowCamera()))->setRumblePercentage(rumble, 0x32);

    if (((Explosion *)(this->explosion))->isPlaying() == 0) {
        ((TargetFollowCamera *)(((PlayerEgo *)(player))->getTargetFollowCamera()))->setRumblePercentage(0.0f, 0);
        this->rumbleTimer = 0;
        this->gun->field_0x88 = 0;
        this->detonationPending = 1;
        ((Explosion *)(this->explosion))->reset();
    }
}

extern "C" void RocketGun_ctor(BombGun *self, int param3, Gun *gun, uint32_t mesh,
                                int zero0, int zero1, int type, bool flag, Level *level);

static const float kCtorBombScale = 50000.0f;
static const float kOffsetY = 450.0f;
static const float kOffsetZ = -1400.0f;
static const float kRocketOffsetZ = 1700.0f;

BombGun *_ZN7BombGunC1EP3GunjiibP5Level(
    BombGun *self, Gun *gun, uint32_t mesh, int param3, int type, int playerControlled,
    Level *level)
{
    char local[12];

    RocketGun_ctor(self, param3, gun, mesh, 0, 0, type, false, level);

    self->cameraOffset = (Vector){0.0f, 0.0f, 0.0f};
    self->cameraTargetOffset = (Vector){0.0f, 0.0f, 0.0f};
    *(void * volatile *)self = (char *)BombGun_vtable + 8;
    self->detonationPosition = (Vector){0.0f, 0.0f, 0.0f};

    Explosion *explosion = (Explosion *)::operator new(0x68);
    int explosionType;
    if (type == 6) {
        explosionType = 7;
    } else if (type == 0x2a) {
        explosionType = 0xb;
    } else {
        explosionType = 0;
        if (gun->itemIndex == 0xe8)
            explosionType = 0xd;
    }
    Explosion_ctor(explosion, explosionType);
    self->explosion = explosion;
    ((Explosion *)(explosion))->setWeaponIndex(gun->itemIndex);

    int playerFlag = *(volatile int *)&playerControlled;
    self->playerControlled = playerFlag;
    bool scaledBomb = type == 0x2a;
    self->detonationPending = 1;
    self->bombType = type;
    self->geometryTransformId = -1;
    if (scaledBomb)
        ((Explosion *)(self->explosion))->setScaling(kCtorBombScale);

    AEGeometry *geometry;
    if (playerFlag) {
        void **canvasHolder = BombGun_player_canvas;
        uint32_t *createdTransform = &self->meshTransformId;
        AbyssEngine::PaintCanvas::TransformCreate(*canvasHolder, createdTransform);
        AbyssEngine::PaintCanvas::TransformAddMesh(*canvasHolder, *createdTransform,
                                     self->meshId, 0);
        AbyssEngine::Transform *transform =
            AbyssEngine::PaintCanvas::TransformGetTransform(*canvasHolder, *createdTransform);
        ((AbyssEngine::Transform *)(transform))->SetAnimationState((AbyssEngine::AnimationMode)1, 0);

        geometry = (AEGeometry *)::operator new(0xc0);
        new ((void *)geometry) AEGeometry((uint16_t)0x37d6, (PaintCanvas *)*canvasHolder, false);
        AbyssEngine::PaintCanvas::TransformAddChild(*canvasHolder, *createdTransform,
                                      geometry->transform);
        AbyssEngine::PaintCanvas::TransformRemoveMesh(*canvasHolder, self->transformId,
                                        self->meshId);
        AbyssEngine::PaintCanvas::TransformAddChild(*canvasHolder, self->transformId,
                                      self->meshTransformId);
    } else {
        int weapon = gun->itemIndex;
        bool normal = weapon != 0xe8;
        if (normal)
            weapon = gun->weaponType;
        if (!normal || weapon == 0x22)
            goto after_geometry;

        geometry = (AEGeometry *)::operator new(0xc0);
        uint16_t geomMesh = 0x395d;
        void **canvasHolder = BombGun_canvas;
        if (mesh == 0x395a)
            geomMesh = 0x395b;
        if (mesh == 0x3958)
            geomMesh = 0x3959;

        new ((void *)geometry) AEGeometry((uint16_t)geomMesh, (PaintCanvas *)*canvasHolder, false);
        self->geometryTransformId = geometry->transform;
        AbyssEngine::PaintCanvas::TransformAddChild(*canvasHolder, self->transformId,
                                      geometry->transform);

        AbyssEngine::Transform *transform;
        if (mesh == 0x395c) {
            transform = AbyssEngine::PaintCanvas::TransformGetTransform(*canvasHolder, self->geometryTransformId);
            ((AbyssEngine::Transform *)(transform))->SetAnimationState((AbyssEngine::AnimationMode)2, 0);
        } else {
            transform = AbyssEngine::PaintCanvas::TransformGetTransform(*canvasHolder, self->transformId);
            ((AbyssEngine::Transform *)(transform))->SetAnimationState((AbyssEngine::AnimationMode)1, 0);
        }
    }

    [&]{ AEGeometry *g_ = (AEGeometry *)(geometry); if (g_) { g_->~AEGeometry(); ::operator delete(g_); } }();

after_geometry:
    *(volatile float *)(local + 0x4) = kOffsetY;
    *(volatile float *)(local + 0x0) = 0.0f;
    *(volatile float *)(local + 0x8) = kOffsetZ;
    self->cameraOffset = *(const Vector *)(local);

    *(volatile float *)(local + 0x0) = 0.0f;
    *(volatile float *)(local + 0x4) = 0.0f;
    *(volatile float *)(local + 0x8) = kRocketOffsetZ;
    self->cameraTargetOffset = *(const Vector *)(local);

    self->player = 0;
    AEGeometry *finalGeometry = (AEGeometry *)::operator new(0xc0);
    void **finalCanvas = BombGun_final_canvas;
    new ((void *)finalGeometry) AEGeometry((PaintCanvas *)*finalCanvas);
    self->trailGeometry = finalGeometry;
    return self;
}
