#include "gof2/BombGun.h"
#include "gof2/FModSound.h"
#include "gof2/LevelScript.h"
#include "gof2/RocketGun.h"
#include "gof2/Status.h"
#include "gof2/Transform.h"
#include "gof2/Explosion.h"
#include "gof2/Gun.h"
#include "gof2/AEGeometry.h"

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
extern "C" void *Vector_assign(void *dst, void *src);
namespace AbyssEngine { namespace PaintCanvas {
AbyssEngine::Transform *TransformGetTransform(void *canvas, uint32_t transform);
void *TransformGetLocal(void *canvas, uint32_t transform);
void TransformCreate(void *canvas, uint32_t *out);
void TransformAddChild(void *canvas, uint32_t parent, uint32_t child);
void TransformRemoveMesh(void *canvas, uint32_t transform, uint16_t mesh);
void TransformAddMesh(void *canvas, uint32_t transform, uint16_t mesh, int flags);
} }
extern "C" void Matrix_assign(void *dst, void *src);
extern "C" void AEGeometry_setMatrix(AEGeometry *self, void *matrix);
extern "C" void Vector_scale_scalar(void *out, float scale, void *vec);
extern "C" void Vector_add(void *out, void *a, void *b);
extern "C" void TargetFollowCamera_setTarget(TargetFollowCamera *self, AEGeometry *target);
extern "C" void TargetFollowCamera_setCamOffset(TargetFollowCamera *self, void *offset);
extern "C" void TargetFollowCamera_setTargetOffset(TargetFollowCamera *self, void *offset);
extern "C" void TargetFollowCamera_useTargetsUpVector(TargetFollowCamera *self, bool enabled);
extern "C" void Vector_sub(void *out, void *a, void *b);
float VectorLength(void *self);
extern "C" void Explosion_update(Explosion *self, int elapsed, TargetFollowCamera *camera);
extern "C" __attribute__((visibility("hidden"))) void **BombGun_player_canvas;
extern "C" __attribute__((visibility("hidden"))) void **BombGun_canvas;
extern "C" __attribute__((visibility("hidden"))) void **BombGun_final_canvas;
extern "C" void Explosion_ctor(Explosion *self, int type);
extern "C" void Explosion_setWeaponIndex(Explosion *self, int index);
extern "C" void AEGeometry_ctor(AEGeometry *self, uint16_t mesh, void *canvas, bool flag);
extern "C" void *AEGeometry_dtor(AEGeometry *self);
extern "C" void AEGeometry_ctor_canvas(AEGeometry *self, void *canvas);

// ---- _BombGun_147824.cpp ----
void *_ZN7BombGunD1Ev(BombGun *self)
{
    *(void **)self = (char *)BombGun_vtable + 8;

    Explosion *explosion = self->field_0xf0;
    if (explosion != 0)
        ::operator delete(Explosion_dtor(explosion));

    self->field_0xf0 = 0;
    return BombGun_base_dtor(self);
}

// ---- _BombGun_147858.cpp ----
void _ZN7BombGunD0Ev(BombGun *self)
{
    return ::operator delete(_ZN7BombGunD1Ev(self));
}

// ---- setPlayer_147868.cpp ----
void BombGun::setPlayer(PlayerEgo *player)
{
    this->field_0xec = player;
}

// ---- render_147cf0.cpp ----
void BombGun::render()
{
    ((RocketGun *)(this))->render();
    if (this->field_0x8->field_0x88 != 0)
        return ((Explosion *)(this->field_0xf0))->render();
}

// ---- update_147870.cpp ----
extern "C" __attribute__((visibility("hidden"))) TargetFollowCamera *(*BombGun_getCamera)(
    PlayerEgo *player);

extern "C" void TargetFollowCamera_setRumblePercentage(TargetFollowCamera *self, float pct,
                                                        int duration);

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

    PlayerEgo *player = this->field_0xec;
    if (player == 0)
        return;

    Gun *gun = this->field_0x8;
    if (gun->field_0x88 == 0) {
        if (this->field_0x128 == 0x2a) {
            ((PlayerEgo *)(position))->getPosition();
        } else {
            void *gunPos = gun->positions;
            *(uint64_t *)position = *(uint64_t *)gunPos;
            *(uint32_t *)(position + 8) = *(uint32_t *)((char *)gunPos + 0x8);
        }
        Vector_assign((char *)this + 0xf8, position);
    }

    gun = this->field_0x8;
    bool playerControlled = this->field_0x24 != 0;
    if (!playerControlled) {
        if (gun->field_0x4c != 0)
            goto update_transforms;
    } else if (gun->field_0x4c != 0) {
        if (gun->field_0x88 != 0)
            goto update_transforms;
        {
            void **canvas = BombGun_update_canvas_b;
            uint32_t *activeTransform = &this->field_0x10;
            if (gun->field_0x4d != 0) {
                gun->field_0x4d = 0;
                AbyssEngine::Transform *transform = AbyssEngine::PaintCanvas::TransformGetTransform(*canvas, *activeTransform);
                ((AbyssEngine::Transform *)(transform))->SetAnimationState((AbyssEngine::AnimationMode)3, 0);
                transform = AbyssEngine::PaintCanvas::TransformGetTransform(*canvas, *activeTransform);
                ((AbyssEngine::Transform *)(transform))->SetAnimationState((AbyssEngine::AnimationMode)1, 0);
                Matrix_assign((char *)player + 0x10, (char *)player->field_0x0 + 4);
                ((FModSound *)(*BombGun_sound_play))->play(0x45c, 0, 0, 0.0f);
            }

            AEGeometry *geometry = this->field_0xe8;
            void *local = AbyssEngine::PaintCanvas::TransformGetLocal(*canvas, *activeTransform);
            AEGeometry_setMatrix(geometry, local);
            Vector_scale_scalar(scaled, kRocketOffsetScale, gun->velocities);
            Vector_add(position, gun->positions, scaled);
            ((AEGeometry *)(geometry))->setPosition(*(Vector *)position);
            ((AEGeometry *)(geometry))->updateReferenceMatrix();

            TargetFollowCamera *camera = BombGun_getCamera(player);
            TargetFollowCamera_setTarget(camera, geometry);
            camera = BombGun_getCamera(player);
            TargetFollowCamera_setCamOffset(camera, (char *)this + 0x110);
            camera = BombGun_getCamera(player);
            TargetFollowCamera_setTargetOffset(camera, (char *)this + 0x11c);
            camera = BombGun_getCamera(player);
            TargetFollowCamera_useTargetsUpVector(camera, false);
            ((PlayerEgo *)(player))->setRocketControl(gun, geometry);

            if (*(int *)gun->lifetimes < gun->initialLifetime - 500) {
                AbyssEngine::Transform *transform = AbyssEngine::PaintCanvas::TransformGetTransform(*canvas, *activeTransform);
                ((AbyssEngine::Transform *)(transform))->Update((int64_t)elapsed, 0);
            }

            float bank = ((PlayerEgo *)(player))->getRocketBanking() * kBombScale;
            this->field_0x20 = bank;
            ((FModSound *)(*BombGun_sound_param))->setParamValue(0, 0x45c, bank * 0.5f);
        }
    }
    goto after_transforms;

update_transforms:
    {
        void **canvas = BombGun_update_canvas_a;
        AbyssEngine::Transform *transform = AbyssEngine::PaintCanvas::TransformGetTransform(*canvas, this->field_0x10);
        ((AbyssEngine::Transform *)(transform))->Update((int64_t)elapsed, 0);
        if (this->field_0xf4 != -1) {
            transform = AbyssEngine::PaintCanvas::TransformGetTransform(*canvas, this->field_0xf4);
            ((AbyssEngine::Transform *)(transform))->Update((int64_t)elapsed, 0);
        }
    }

after_transforms:

    ((RocketGun *)(this))->update(elapsed);
    gun = this->field_0x8;
    if (gun->field_0x88 == 0)
        return;

    if (this->field_0x104 != 0) {
        if (this->field_0x24 != 0) {
            player = this->field_0xec;
            ((LevelScript *)(player->field_0x10))->resetCamera(player->field_0xc);
            ((PlayerEgo *)(player))->setRocketControl(0, this->field_0xe8);
            ((FModSound *)(*BombGun_sound_stop))->stop(0x45c);
        }

        this->field_0x108 = 0;
        player = this->field_0xec;
        ((PlayerEgo *)(work))->getPosition();
        Vector_sub(position, (char *)this + 0xf8, work);
        float distance = VectorLength(position);
        float magnitude = (float)((Gun *)(this->field_0x8))->getMagnitude();
        float force = (((magnitude * 0.5f) - distance) / (magnitude * 0.5f)) * 0.5f;
        if (force > 1.0f)
            force = 1.0f;
        if (force < kZero)
            force = kZero;
        if (this->field_0x128 == 0x2a)
            force *= kBombScale;
        if (((Status *)(*BombGun_status))->hardCoreMode() != 0) {
            Gun *damageGun = this->field_0x8;
            ((Player *)((Player *)player->field_0x0))->damage((int)(force * (float)damageGun->field_0x60));
        }
        ((PlayerEgo *)(player))->addNukeVolatileForce(force);

        float capped = kRumbleDistance;
        if (distance < kRumbleDistance)
            capped = distance;
        this->field_0x10c = 1.0f - capped / kRumbleDistance;
        TargetFollowCamera_setRumblePercentage(((PlayerEgo *)(player))->getTargetFollowCamera(),
                                               this->field_0x10c, 0x32);

        Explosion *explosion = this->field_0xf0;
        if (this->field_0x128 == 0x2a) {
            ((PlayerEgo *)(position))->GetDirVector();
        } else {
            *(uint64_t *)position = 0;
            *(uint32_t *)(position + 8) = 0;
        }
        ((Explosion *)(explosion))->start((const Vector *)((char *)this + 0xf8),
                                          (const Vector *)position);
        this->field_0x104 = 0;
    }

    Explosion_update(this->field_0xf0, elapsed, 0);
    int timer = this->field_0x108 + elapsed;
    if (timer > 2000)
        timer = 2000;
    this->field_0x108 = timer;

    player = this->field_0xec;
    float rumble = this->field_0x10c * ((float)timer / kRumbleTime + 1.0f);
    TargetFollowCamera_setRumblePercentage(((PlayerEgo *)(player))->getTargetFollowCamera(), rumble, 0x32);

    if (((Explosion *)(this->field_0xf0))->isPlaying() == 0) {
        TargetFollowCamera_setRumblePercentage(((PlayerEgo *)(player))->getTargetFollowCamera(), 0.0f, 0);
        this->field_0x108 = 0;
        this->field_0x8->field_0x88 = 0;
        this->field_0x104 = 1;
        ((Explosion *)(this->field_0xf0))->reset();
    }
}

// ---- BombGun_1475cc.cpp ----
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

    self->field_0x110 = (v4i){0, 0, 0, 0};
    *(void * volatile *)self = (char *)BombGun_vtable + 8;
    self->field_0xf8 = 0;
    self->field_0x100 = 0;
    self->field_0x120 = 0;

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
    self->field_0xf0 = explosion;
    Explosion_setWeaponIndex(explosion, gun->itemIndex);

    int playerFlag = *(volatile int *)&playerControlled;
    self->field_0x24 = playerFlag;
    bool scaledBomb = type == 0x2a;
    self->field_0x104 = 1;
    self->field_0x128 = type;
    self->field_0xf4 = -1;
    if (scaledBomb)
        ((Explosion *)(self->field_0xf0))->setScaling(kCtorBombScale);

    AEGeometry *geometry;
    if (playerFlag) {
        void **canvasHolder = BombGun_player_canvas;
        uint32_t *createdTransform = &self->field_0x14;
        AbyssEngine::PaintCanvas::TransformCreate(*canvasHolder, createdTransform);
        AbyssEngine::PaintCanvas::TransformAddMesh(*canvasHolder, *createdTransform,
                                     self->field_0x28, 0);
        AbyssEngine::Transform *transform =
            AbyssEngine::PaintCanvas::TransformGetTransform(*canvasHolder, *createdTransform);
        ((AbyssEngine::Transform *)(transform))->SetAnimationState((AbyssEngine::AnimationMode)1, 0);

        geometry = (AEGeometry *)::operator new(0xc0);
        AEGeometry_ctor(geometry, 0x37d6, *canvasHolder, false);
        AbyssEngine::PaintCanvas::TransformAddChild(*canvasHolder, *createdTransform,
                                      geometry->transform);
        AbyssEngine::PaintCanvas::TransformRemoveMesh(*canvasHolder, self->field_0x10,
                                        self->field_0x28);
        AbyssEngine::PaintCanvas::TransformAddChild(*canvasHolder, self->field_0x10,
                                      self->field_0x14);
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

        AEGeometry_ctor(geometry, geomMesh, *canvasHolder, false);
        self->field_0xf4 = geometry->transform;
        AbyssEngine::PaintCanvas::TransformAddChild(*canvasHolder, self->field_0x10,
                                      geometry->transform);

        AbyssEngine::Transform *transform;
        if (mesh == 0x395c) {
            transform = AbyssEngine::PaintCanvas::TransformGetTransform(*canvasHolder, self->field_0xf4);
            ((AbyssEngine::Transform *)(transform))->SetAnimationState((AbyssEngine::AnimationMode)2, 0);
        } else {
            transform = AbyssEngine::PaintCanvas::TransformGetTransform(*canvasHolder, self->field_0x10);
            ((AbyssEngine::Transform *)(transform))->SetAnimationState((AbyssEngine::AnimationMode)1, 0);
        }
    }

    ::operator delete(AEGeometry_dtor(geometry));

after_geometry:
    *(volatile float *)(local + 0x4) = kOffsetY;
    *(volatile float *)(local + 0x0) = 0.0f;
    *(volatile float *)(local + 0x8) = kOffsetZ;
    Vector_assign((char *)self + 0x110, local);

    *(volatile float *)(local + 0x0) = 0.0f;
    *(volatile float *)(local + 0x4) = 0.0f;
    *(volatile float *)(local + 0x8) = kRocketOffsetZ;
    Vector_assign((char *)self + 0x11c, local);

    self->field_0xec = 0;
    AEGeometry *finalGeometry = (AEGeometry *)::operator new(0xc0);
    void **finalCanvas = BombGun_final_canvas;
    AEGeometry_ctor_canvas(finalGeometry, *finalCanvas);
    self->field_0xe8 = finalGeometry;
    return self;
}
