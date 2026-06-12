#include "gof2/RocketGun.h"
#include "gof2/AEGeometry.h"
#include "gof2/Level.h"
#include "gof2/ParticleSystemManager.h"
#include "gof2/PlayerEgo.h"
#include "gof2/Gun.h"
#include "gof2/Player.h"
#include "gof2/Radar.h"

// Radar.h defines a competing AbyssEngine::PaintCanvas stub, so the real
// PaintCanvas headers cannot be co-included here. Complete the global
// ::PaintCanvas (forward-declared by AEGeometry.h) with only the methods this
// TU calls; signatures are byte-identical to PaintCanvas.h so the mangled
// ::PaintCanvas:: symbols resolve to the real definitions.
class PaintCanvas {
public:
    void TransformAddChild(unsigned int parent, unsigned int child);
    void *TransformGetLocal(unsigned int index);
    void TransformSetLocal(unsigned int index, const Matrix &matrix);
};


extern "C" void RocketGun_render_tail();
extern "C" __attribute__((visibility("hidden"))) void *RocketGun_vtable;
extern "C" void *Array_Matrix_dtor(void *p);
extern "C" void *Array_int_dtor(void *p);
extern "C" void *RocketGun_base_dtor(RocketGun *self);
void *_ZN9RocketGunD1Ev(RocketGun *self);
extern "C" __attribute__((visibility("hidden"))) void **RocketGun_canvas_holder;
extern "C" __attribute__((visibility("hidden"))) void **RocketGun_canvas_holder2;
extern "C" void Array_Matrix_ctor(void *self);
extern "C" void Array_int_ctor(void *self);
extern "C" void ArraySetLength_Matrix(uint32_t length, void *array);
extern "C" void ArraySetLength_int(uint32_t length, void *array);
extern "C" void ParticleSystemManager_attachSystem(int manager, int system, int zero);
extern "C" __attribute__((visibility("hidden"))) void (*RocketGun_vector_func)(void *out, void *in);
extern "C" int Player_getEnemies(void *player);
extern "C" __attribute__((visibility("hidden"))) void **RocketGun_canvas_holder3;
extern "C" __attribute__((visibility("hidden"))) void (*RocketGun_vector_func2)(void *out, void *in);
extern "C" __attribute__((visibility("hidden"))) void (*RocketGun_vector_scale_func)(void *out, void *in, float scale);
void MatrixRotateVector(void *out, void *matrix, void *vec);
void MatrixGetDir(void *out, void *matrix);
void MatrixSetTranslation(void *matrix, float x, float y, float z);
extern "C" void *__aeabi_memcpy(void *dst, const void *src, uint32_t n);
void VectorCross(void *out, void *a, void *b);
void VectorNormalize(void *out, void *in);
extern "C" uint32_t __aeabi_uidiv(uint32_t a, uint32_t b);
extern "C" float AEMath_Sinf(float v);
extern "C" float AEMath_Cosf(float v);

namespace AbyssEngine {
namespace AEMath {
Vector operator+(const Vector &a, const Vector &b);
Vector operator-(const Vector &a, const Vector &b);
Vector operator*(const Vector &v, float scale);
}
}

// ---- render_15ecd4.cpp ----
void RocketGun::render()
{
    return RocketGun_render_tail();
}

// ---- _RocketGun_15e9d0.cpp ----
void *_ZN9RocketGunD1Ev(RocketGun *self)
{
    *(void **)self = (char *)RocketGun_vtable + 8;

    void *a0 = self->trailMatrices;
    if (a0 != 0)
        ::operator delete(Array_Matrix_dtor(a0));

    void *a1 = self->trailSystems;
    self->trailMatrices = 0;
    if (a1 != 0)
        ::operator delete(Array_int_dtor(a1));

    void *a2 = self->trailTimers;
    self->trailSystems = 0;
    if (a2 != 0)
        ::operator delete(Array_int_dtor(a2));

    self->trailTimers = 0;
    return RocketGun_base_dtor(self);
}

// ---- _RocketGun_15ea28.cpp ----
void _ZN9RocketGunD0Ev(RocketGun *self)
{
    return ::operator delete(_ZN9RocketGunD1Ev(self));
}

// ---- RocketGun_15e8f8.cpp ----
extern "C" void ObjectGun_ctor(RocketGun *self, int param_1, Gun *param_2, int param_3,
                                uint32_t param_5, Level *param_8);

static const uint32_t kRocketC8 = 0x3dcccccd;

RocketGun::RocketGun(int param_1, Gun *param_2, int param_3, int param_4,
                     uint32_t param_5, int param_6, bool param_7, Level *param_8)
{
    ObjectGun_ctor(this, param_1, param_2, param_3, param_5, param_8);

    void *vtable = RocketGun_vtable;
    char *zeroBase = (char *)this + 0xd4;
    uint32_t c8 = kRocketC8;
    *(v4i *)zeroBase = (v4i){0, 0, 0, 0};
    this->steerX = 0;
    this->steerY = 0;
    this->steerZ = 0;
    this->homing = param_7;
    this->field_0xc4 = -1;
    this->turnRate = c8;
    this->particleSystem = -1;
    this->rocketKind = param_6;
    this->particleManager = 0;
    *(void **)this = (char *)vtable + 8;
    this->field_0x4 = -1;

    int gate = 0x37a9;
    if (param_3 != 0x37a9)
        gate = 0x37a7;
    if (param_3 == 0x37a9 || param_3 == gate) {
        void *geom = ::operator new(0xc0);
        uint16_t mesh = 0x37aa;
        void **holder = RocketGun_canvas_holder;
        if (param_1 == 0x37a7)
            mesh = 0x37a8;
        new ((void *)geom) AEGeometry((uint16_t)mesh, (PaintCanvas *)*holder, false);
        ((PaintCanvas*)*holder)->TransformAddChild(this->transformId,
                                      F<uint32_t>(geom, 0xc));
        [&]{ AEGeometry *g_ = (AEGeometry *)(geom); if (g_) { g_->~AEGeometry(); ::operator delete(g_); } }();
    }
}

// ---- setRadar_15ea38.cpp ----
void RocketGun::setRadar(Radar *radar)
{
    this->radar = radar;
    void *radarObj = radar->level;
    Gun *gun = this->gun;
    this->particleManager = F<int>(radarObj, 0x80);

    int gunType = gun->itemIndex;
    uint32_t mode = (uint32_t)(gunType - 0x1c);
    if (mode > 2) {
        if (gunType != 0xc1) {
            int altType = gun->weaponType;
            if (altType != 0x28)
                goto non_special;
        }
    }
    if (gun->weaponType == 0x28) {
        mode = 0;
    } else if (gunType == 0xc1) {
        mode = 3;
    }

    {
        void *matrices = ::operator new(0xc);
        Array_Matrix_ctor(matrices);
        this->trailMatrices = matrices;
        void *systems = ::operator new(0xc);
        Array_int_ctor(systems);
        this->trailSystems = systems;
        void *timers = ::operator new(0xc);
        Array_int_ctor(timers);
        this->trailTimers = timers;

        uint32_t count = F<uint32_t>(this->gun, 0x8);
        ArraySetLength_Matrix(count, this->trailMatrices);
        ArraySetLength_int(F<uint32_t>(this->gun, 0x8), this->trailSystems);
        ArraySetLength_int(F<uint32_t>(this->gun, 0x8), this->trailTimers);

        int defaultType = 0x1c;
        if (mode == 2)
            defaultType = 0x1b;
        uint32_t zero = 0;
        int matrixOff = 0;
        for (uint32_t i = 0; i < F<uint32_t>(this->gun, 0x8); i++) {
            int manager;
            if (F<int>(this->gun, 0x58) == 0xc1) {
                manager = F<int>(radar->level, 0x98);
                this->particleManager = manager;
            } else {
                manager = this->particleManager;
            }

            int effect;
            if (mode == 0) {
                effect = 0x19;
            } else {
                effect = defaultType;
                if (mode == 1)
                    effect = 0x1a;
            }

            int system = ((ParticleSystemManager *)(manager))->addSystem((void *)(F<int>(this->trailMatrices, 0x4) + matrixOff), effect, false);
            *(int *)(F<char *>(this->trailSystems, 0x4) + i * 4) = system;
            ((ParticleSystemManager *)(this->particleManager))->enableSystemEmit(system, false);
            matrixOff += 0x3c;
            *(uint32_t *)(F<char *>(this->trailTimers, 0x4) + i * 4) = zero;
        }
        return;
    }

non_special:
    int rocketKind = this->rocketKind;
    if ((uint32_t)(rocketKind - 4) < 2 || rocketKind == 0x28) {
        void *matrices = ::operator new(0xc);
        Array_Matrix_ctor(matrices);
        this->trailMatrices = matrices;
        void *systems = ::operator new(0xc);
        Array_int_ctor(systems);
        this->trailSystems = systems;
        void *timers = ::operator new(0xc);
        Array_int_ctor(timers);
        this->trailTimers = timers;

        ArraySetLength_Matrix(F<uint32_t>(this->gun, 0x8), this->trailMatrices);
        ArraySetLength_int(F<uint32_t>(this->gun, 0x8), this->trailSystems);
        ArraySetLength_int(F<uint32_t>(this->gun, 0x8), this->trailTimers);

        uint32_t zero = 0;
        int matrixOff = 0;
        for (uint32_t i = 0; i < F<uint32_t>(this->gun, 0x8); i++) {
            int system = ((ParticleSystemManager *)(F<int>(radar->level, 0x80)))->addSystem((void *)(F<int>(this->trailMatrices, 0x4) + matrixOff), 0x27, false);
            *(int *)(F<char *>(this->trailSystems, 0x4) + i * 4) = system;
            ((ParticleSystemManager *)(F<int>(radar->level, 0x80)))->enableSystemEmit(system, false);
            matrixOff += 0x3c;
            *(uint32_t *)(F<char *>(this->trailTimers, 0x4) + i * 4) = zero;
        }
        return;
    }

    if (gunType == 0xe8) {
        int manager = F<int>(radarObj, 0x9c);
        void **holder = RocketGun_canvas_holder2;
        int local = (int)(long)((PaintCanvas*)*holder)->TransformGetLocal(this->transformId);
        int system = ((ParticleSystemManager *)(manager))->addSystem((void *)local, 0x2f, false);
        this->particleSystem = system;
        ParticleSystemManager_attachSystem(F<int>(radar->level, 0x9c), system, 0);
    } else {
        int manager = F<int>(radarObj, 0x84);
        void **holder = RocketGun_canvas_holder2;
        int local = (int)(long)((PaintCanvas*)*holder)->TransformGetLocal(this->transformId);
        int system = ((ParticleSystemManager *)(manager))->addSystem((void *)local, 0xc, false);
        this->particleSystem = system;
        ParticleSystemManager_attachSystem(F<int>(radar->level, 0x84), system, 0);
    }
}

// ---- seekEnemy_15f44c.cpp ----
void RocketGun::seekEnemy(int unused, int index)
{
    char tmp1[12];
    char tmp0[12];
    char enemyPos[12];

    void *hasEnemies = ((Gun *)(this->gun))->getEnemies();
    void *enemy;

    if (F<void *>(this->gun, 0x4) == 0)
        goto fallback;
    if (((Gun *)(this->gun))->isPlayerGun() != 0)
        goto fallback;
    if (F<void *>(this->gun, 0x4) == 0)
        goto fallback;
    enemy = F<void *>(this->gun, 0x4);
    if (F<int>(((Player *)(enemy))->getKIPlayer(), 0x38) < 0)
        goto fallback;
    if (Player_getEnemies(F<void *>(this->gun, 0x4)) == 0)
        goto fallback;
    enemy = F<void *>(this->gun, 0x4);
    enemy = ((Player *)(enemy))->getEnemy(F<int>(((Player *)(enemy))->getKIPlayer(), 0x38));
    goto have_enemy;

fallback:
    if (hasEnemies == 0)
        return;
    if (this->radar == 0)
        return;
    enemy = F<void *>(this->radar, 0x4);
    if (enemy == 0)
        return;
    if (F<uint8_t>(enemy, 0x76) == 0 || F<uint8_t>(enemy, 0x74) != 0)
        return;
    if (((PlayerEgo *)(((Level *)(F<void *>(this->radar, 0x0)))->getPlayer()))->isInFreeLookMode() != 0)
        return;
    enemy = F<void *>(F<void *>(this->radar, 0x4), 0x4);

have_enemy:
    if (enemy != 0) {
        ((Player *)(enemy))->getPosition((Vector *)(enemyPos));
        *(Vector *)(tmp0) = *(const Vector *)(enemyPos) - *(const Vector *)((char *)F<void *>(this->gun, 0xc) + index * 0xc);
        void (*fn)(void *, void *) = RocketGun_vector_func;
        fn(tmp1, tmp0);
        void *steer = (char *)this + 0xb4;
        *(Vector *)(steer) = *(const Vector *)(tmp1);
        fn(tmp1, (char *)F<void *>(this->gun, 0x18) + index * 0xc);
        *(Vector *)(steer) -= *(const Vector *)(tmp1);
        *(Vector *)(steer) /= (this->turnRate * 20.0f);
        *(Vector *)(tmp1) += *(const Vector *)(steer);
        fn(tmp0, tmp1);
        *(Vector *)((char *)F<void *>(this->gun, 0x18) + index * 0xc) = *(const Vector *)(tmp0);
        *(Vector *)((char *)F<void *>(this->gun, 0x18) + index * 0xc) *= (F<float>(this->gun, 0x50));
    }
}

// ---- update_15ecd8.cpp ----
static const float kMuzzleZAdd = 0.0001f;
static const float kScaleDiv = 1000.0f;
static const float kScaleMul = 100.0f;
static const float kZeroCompare = 0.0f;
static const float kWave = 0.001f;

void RocketGun::update(int elapsed)
{
    char norm[12];
    char matrix[0x3c];
    char axis[12];
    char dir[12];
    char gunVec[12];
    char pos[12];
    char tmp[12];
    Vector zero = {0.0f, 0.0f, 0.0f};

    ((Gun *)(this->gun))->update(elapsed);

    if (this->field_0x1c != 0 && F<uint8_t>(this->gun, 0xa9) != 0) {
        void *player = (void *)((Level *)(this->level))->getPlayer();
        ((PlayerEgo *)(matrix))->getPosition();
        *(uint32_t *)gunVec = F<uint32_t>(this->gun, 0x7c);
        *(uint32_t *)(gunVec + 4) = F<uint32_t>(this->gun, 0x80);
        *(float *)(gunVec + 8) = F<float>(this->gun, 0x84) + kMuzzleZAdd;
        MatrixRotateVector(axis, (char *)F<void *>(player, 0x0) + 4, gunVec);
        *(Vector *)(matrix) += *(const Vector *)(axis);
        ((AEGeometry *)(this->geometry))->setPosition(*(Vector *)matrix);

        float scaling = (float)F<int>(this->gun, 0x70);
        scaling = (float)(int)((scaling / kScaleDiv + 1.0f) * kScaleMul);
        scaling = scaling / kScaleMul;
        ((AEGeometry *)(this->geometry))->setScaling(scaling);

        MatrixGetDir(axis, (char *)F<void *>(player, 0x0) + 4);
        zero.x = 0.0f;
        zero.y = 1.0f;
        zero.z = 0.0f;
        ((AEGeometry *)(this->geometry))->setDirection(*(Vector *)axis, zero);
    }

    Gun *gun = this->gun;
    this->field_0x1d = gun->delayActive;

    if (gun->field_0x4d != 0) {
        this->field_0xc4 = -1;
        this->fadeTimer = 0;
        gun->field_0x4d = 0;

        void **holder = RocketGun_canvas_holder3;
        int local = (int)(long)((PaintCanvas*)*holder)->TransformGetLocal(this->transformId);
        int shot = F<int>(this->gun, 0xa0);
        char *basePos = (char *)F<void *>(this->gun, 0xc) + shot * 0xc;
        MatrixSetTranslation(matrix, *(float *)basePos, *(float *)(basePos + 4), *(float *)(basePos + 8));

        char *vel = (char *)F<void *>(this->gun, 0x18) + shot * 0xc;
        *(uint32_t *)gunVec = *(uint32_t *)vel;
        *(uint32_t *)(gunVec + 4) = *(uint32_t *)(vel + 4);
        *(uint32_t *)(gunVec + 8) = *(uint32_t *)(vel + 8);

        local = (int)(long)((PaintCanvas*)*holder)->TransformGetLocal(this->transformId);
        __aeabi_memcpy(matrix, (void *)local, 0x3c);
        *(float *)axis = 0.0f;
        *(float *)(axis + 4) = 1.0f;
        *(float *)(axis + 8) = 0.0f;
        VectorCross(dir, axis, gunVec);
        VectorNormalize(norm, dir);
        void (*fn)(void *, void *) = RocketGun_vector_func2;
        fn(dir, norm);
        VectorCross(norm, gunVec, dir);
        fn(axis, norm);
        VectorNormalize(norm, axis);
        fn(axis, norm);

        *(uint32_t *)(matrix + 0x00) = *(uint32_t *)dir;
        *(uint32_t *)(matrix + 0x04) = *(uint32_t *)axis;
        *(uint32_t *)(matrix + 0x08) = *(uint32_t *)gunVec;
        *(uint32_t *)(matrix + 0x10) = *(uint32_t *)(dir + 4);
        *(uint32_t *)(matrix + 0x14) = *(uint32_t *)(axis + 4);
        *(uint32_t *)(matrix + 0x18) = *(uint32_t *)(gunVec + 4);
        *(uint32_t *)(matrix + 0x20) = *(uint32_t *)(dir + 8);
        *(uint32_t *)(matrix + 0x24) = *(uint32_t *)(axis + 8);
        *(uint32_t *)(matrix + 0x28) = *(uint32_t *)(gunVec + 8);
        ((PaintCanvas*)*holder)->TransformSetLocal(this->transformId, *(const Matrix *)matrix);

        if (this->trailMatrices == 0) {
            int kind = this->rocketKind;
            if ((uint32_t)(kind - 4) < 2 || kind == 0x28) {
                int manager = F<int>(F<void *>(this->radar, 0x0), 0x80);
                ((ParticleSystemManager *)(manager))->enableSystemRender(this->particleSystem, true);
                ((ParticleSystemManager *)(manager))->enableSystemEmit(this->particleSystem, true);
            } else {
                int manager;
                if (F<int>(this->gun, 0x58) == 0xe8)
                    manager = F<int>(F<void *>(this->radar, 0x0), 0x9c);
                else
                    manager = F<int>(F<void *>(this->radar, 0x0), 0x84);
                ((ParticleSystemManager *)(manager))->enableSystemEmit(this->particleSystem, this->particleSystem != 0);
            }
        } else {
            int shotIndex = F<int>(this->gun, 0xa0);
            char *shotPos = (char *)F<void *>(this->gun, 0xc) + shotIndex * 0xc;
            int system = *(int *)(F<char *>(this->trailSystems, 0x4) + shotIndex * 4);
            if (*(float *)(shotPos + 8) != kZeroCompare) {
                *(Matrix *)(F<char *>(this->trailMatrices, 0x4) + shotIndex * 0x3c) = *(const Matrix *)(matrix);
                ((ParticleSystemManager *)(this->particleManager))->resetSystem(system);
                ((ParticleSystemManager *)(this->particleManager))->enableSystemEmit(system, true);
                ((ParticleSystemManager *)(this->particleManager))->enableSystemRender(system, true);
                *(int *)(F<char *>(this->trailTimers, 0x4) + shotIndex * 4) = 0;
            } else {
                ((ParticleSystemManager *)(this->particleManager))->enableSystemEmit(system, false);
                ((ParticleSystemManager *)(this->particleManager))->resetSystem(system);
                *(int *)(F<char *>(this->trailTimers, 0x4) +
                         F<int>(this->gun, 0xa0) * 4) = 0;
            }
        }

        gun = this->gun;
        if (gun->weaponType == 0x28 && gun->field_0xa0 > 0) {
            gun->field_0x4d = 1;
            gun->field_0xa0 = gun->field_0xa0 - 1;
            typedef void (*Fn)(RocketGun *, int);
            Fn fn = *(Fn *)(*(char **)this + 0x10);
            fn(this, elapsed);
            ((Gun *)(this->gun))->update(elapsed);
            gun = this->gun;
        }
    }

    if (gun->field_0x4c != 0 && gun->count == 1 &&
        *(float *)((char *)gun->positions + 8) == kZeroCompare) {
        gun->field_0x4c = 0;
        int kind = this->rocketKind;
        int manager;
        if ((uint32_t)(kind - 4) < 2 || kind == 0x28) {
            if (this->fadeTimer == 0)
                this->fadeTimer = 2000;
            manager = F<int>(F<void *>(this->radar, 0x0), 0x80);
        } else {
            if (gun->itemIndex == 0xe8)
                manager = F<int>(F<void *>(this->radar, 0x0), 0x9c);
            else
                manager = F<int>(F<void *>(this->radar, 0x0), 0x84);
        }
        ((ParticleSystemManager *)(manager))->enableSystemEmit(this->particleSystem, false);
    } else if (this->fadeTimer > 0 && gun->count == 1 &&
               *(float *)((char *)gun->positions + 8) == kZeroCompare) {
        int timer = this->fadeTimer - elapsed;
        this->fadeTimer = timer;
        if (timer < 1) {
            this->fadeTimer = 0;
            ((ParticleSystemManager *)(this->particleManager))->enableSystemRender(this->particleSystem, false);
        }
    }

    float dt = (float)elapsed;
    for (uint32_t i = 0, vecOff = 0, matOff = 0; i < F<uint32_t>(this->gun, 0x8);
         i++, vecOff += 0xc, matOff += 0x3c) {
        gun = this->gun;
        *(Vector *)(axis) = *(const Vector *)((char *)gun->velocities + vecOff) * (dt);
        *(Vector *)(gunVec) = *(const Vector *)(axis) * (0.5f);
        *(Vector *)(matrix) = *(const Vector *)((char *)gun->positions + vecOff) + *(const Vector *)(gunVec);
        *(Vector *)(&zero) = *(const Vector *)(matrix);

        gun = this->gun;
        if (gun->field_0x4c != 0) {
            if (zero.z != kZeroCompare) {
                if (this->homing != 0 &&
                    (this->trailMatrices != 0 ||
                     *(int *)((char *)gun->lifetimes + i * 4) < gun->initialLifetime - 1000)) {
                    this->seekEnemy(*(int *)((char *)gun->lifetimes + i * 4), i);
                    gun = this->gun;
                }

                if (gun->weaponType == 0x28) {
                    uint32_t total = gun->initialLifetime;
                    uint32_t base = __aeabi_uidiv(total * i, gun->count);
                    float waveIn = (float)base +
                                   (float)(gun->initialLifetime -
                                           *(int *)((char *)gun->lifetimes + i * 4));
                    float s = AEMath_Sinf(waveIn * kWave);
                    float c = AEMath_Cosf(waveIn * kWave);
                    VectorCross(gunVec, (char *)gun->velocities + vecOff,
                                (char *)gun->field_0x24 + vecOff);
                    VectorNormalize(matrix, gunVec);
                    void (*scaleFn)(void *, void *, float) = RocketGun_vector_scale_func;
                    scaleFn(axis, matrix, s + s);
                    scaleFn(gunVec, axis, dt);
                    *(Vector *)((char *)F<void *>(this->gun, 0xc) + vecOff) += *(const Vector *)(gunVec);
                    scaleFn(axis, (char *)F<void *>(this->gun, 0x24) + vecOff, c + c);
                    scaleFn(gunVec, axis, dt);
                    *(Vector *)((char *)F<void *>(this->gun, 0xc) + vecOff) += *(const Vector *)(gunVec);
                }
            } else {
                if (this->trailMatrices != 0) {
                    int system = *(int *)(F<char *>(this->trailSystems, 0x4) + i * 4);
                    ((ParticleSystemManager *)(this->particleManager))->enableSystemEmit(system, false);
                    int *timer = (int *)(F<char *>(this->trailTimers, 0x4) + i * 4);
                    if (*timer == 0)
                        *timer = 2000;
                }
            }

            if (this->trailMatrices != 0) {
                gun = this->gun;
                *(Vector *)(axis) = *(const Vector *)((char *)gun->velocities + vecOff) * (dt);
                *(Vector *)(gunVec) = *(const Vector *)(axis) * (0.5f);
                *(Vector *)(matrix) = *(const Vector *)((char *)gun->positions + vecOff) + *(const Vector *)(gunVec);
                *(Vector *)(&zero) = *(const Vector *)(matrix);
                MatrixSetTranslation(matrix, zero.x, zero.y, zero.z);
                VectorNormalize(gunVec, (char *)F<void *>(this->gun, 0x18) + vecOff);
                char *m = F<char *>(this->trailMatrices, 0x4) + matOff;
                MatrixSetTranslation(matrix, zero.x, zero.y, zero.z);
                *(uint32_t *)(m + 0x8) = *(uint32_t *)gunVec;
                *(uint32_t *)(m + 0x18) = *(uint32_t *)(gunVec + 4);
                *(uint32_t *)(m + 0x28) = *(uint32_t *)(gunVec + 8);
            }
        }

        if (this->trailTimers != 0 && zero.z == kZeroCompare) {
            int *timer = (int *)(F<char *>(this->trailTimers, 0x4) + i * 4);
            if (*timer > 0) {
                int left = *timer - elapsed;
                *timer = left;
                if (left < 1) {
                    *timer = 0;
                    ((ParticleSystemManager *)(this->particleManager))->enableSystemRender(*(int *)(F<char *>(this->trailSystems, 0x4) + i * 4), false);
                }
            }
        }
    }
}
