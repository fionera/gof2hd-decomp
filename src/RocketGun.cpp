#include "gof2/RocketGun.h"
#include "gof2/PlayerEgo.h"
#include "gof2/Gun.h"
#include "gof2/Player.h"
#include "gof2/Radar.h"


extern "C" void RocketGun_render_tail();
extern "C" __attribute__((visibility("hidden"))) void *RocketGun_vtable;
extern "C" void *Array_Matrix_dtor(void *p);
extern "C" void *Array_int_dtor(void *p);
extern "C" void operator_delete(void *p);
extern "C" void *RocketGun_base_dtor(RocketGun *self);
extern "C" void *_ZN9RocketGunD1Ev(RocketGun *self);
extern "C" __attribute__((visibility("hidden"))) void **RocketGun_canvas_holder;
extern "C" void *operator_new(uint32_t size);
extern "C" void AEGeometry_ctor(void *self, uint16_t mesh, void *canvas, bool flag);
extern "C" void PaintCanvas_TransformAddChild(void *canvas, uint32_t parent, uint32_t child);
extern "C" void *AEGeometry_dtor(void *self);
extern "C" __attribute__((visibility("hidden"))) void **RocketGun_canvas_holder2;
extern "C" void Array_Matrix_ctor(void *self);
extern "C" void Array_int_ctor(void *self);
extern "C" void ArraySetLength_Matrix(uint32_t length, void *array);
extern "C" void ArraySetLength_int(uint32_t length, void *array);
extern "C" int PaintCanvas_TransformGetLocal(void *canvas, uint32_t id);
extern "C" int ParticleSystemManager_addSystem(int manager, int transform, int type, bool flag);
extern "C" void ParticleSystemManager_enableSystemEmit(int manager, int system, bool enabled);
extern "C" void ParticleSystemManager_attachSystem(int manager, int system, int zero);
extern "C" __attribute__((visibility("hidden"))) void (*RocketGun_vector_func)(void *out, void *in);
extern "C" int Player_getEnemies(void *player);
extern "C" void *Level_getPlayer(void *level);
extern "C" void Player_getPosition(void *out, void *player);
extern "C" void Vector_sub(void *out, void *a, void *b);
extern "C" void *Vector_assign(void *dst, void *src);
extern "C" void Vector_isub(void *dst, void *src);
extern "C" void Vector_idiv(void *dst, float value);
extern "C" void Vector_iadd(void *dst, void *src);
extern "C" void Vector_imul(void *dst, float value);
extern "C" __attribute__((visibility("hidden"))) void **RocketGun_canvas_holder3;
extern "C" __attribute__((visibility("hidden"))) void (*RocketGun_vector_func2)(void *out, void *in);
extern "C" __attribute__((visibility("hidden"))) void (*RocketGun_vector_scale_func)(void *out, void *in, float scale);
extern "C" void MatrixRotateVector(void *out, void *matrix, void *vec);
extern "C" void AEGeometry_setPosition(void *geom, void *pos);
extern "C" void AEGeometry_setScaling(void *geom, float x, float y, float z);
extern "C" void MatrixGetDir(void *out, void *matrix);
extern "C" void AEGeometry_setDirection(void *geom, void *dir, void *up);
extern "C" void MatrixSetTranslation(void *matrix, float x, float y, float z);
extern "C" void *__aeabi_memcpy(void *dst, const void *src, uint32_t n);
extern "C" void VectorCross(void *out, void *a, void *b);
extern "C" void VectorNormalize(void *out, void *in);
extern "C" void PaintCanvas_TransformSetLocal(void *canvas, uint32_t transformId, const void *matrix);
extern "C" void Matrix_assign(void *dst, void *src);
extern "C" void ParticleSystemManager_enableSystemRender(int manager, int system, bool enabled);
extern "C" void ParticleSystemManager_resetSystem(int manager, int system);
extern "C" uint32_t __aeabi_uidiv(uint32_t a, uint32_t b);
extern "C" float AEMath_Sinf(float v);
extern "C" float AEMath_Cosf(float v);
extern "C" void Vector_scale(void *out, void *vec, float scale);
extern "C" void Vector_add(void *out, void *a, void *b);

// ---- render_15ecd4.cpp ----
void RocketGun::render()
{
    return RocketGun_render_tail();
}

// ---- _RocketGun_15e9d0.cpp ----
extern "C" void *_ZN9RocketGunD1Ev(RocketGun *self)
{
    *(void **)self = (char *)RocketGun_vtable + 8;

    void *a0 = self->field_0xd8;
    if (a0 != 0)
        operator_delete(Array_Matrix_dtor(a0));

    void *a1 = self->field_0xdc;
    self->field_0xd8 = 0;
    if (a1 != 0)
        operator_delete(Array_int_dtor(a1));

    void *a2 = self->field_0xe0;
    self->field_0xdc = 0;
    if (a2 != 0)
        operator_delete(Array_int_dtor(a2));

    self->field_0xe0 = 0;
    return RocketGun_base_dtor(self);
}

// ---- _RocketGun_15ea28.cpp ----
extern "C" void _ZN9RocketGunD0Ev(RocketGun *self)
{
    return operator_delete(_ZN9RocketGunD1Ev(self));
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
    this->field_0xb4 = 0;
    this->field_0xb8 = 0;
    this->field_0xbc = 0;
    this->field_0xc0 = param_7;
    this->field_0xc4 = -1;
    this->field_0xc8 = c8;
    this->field_0xcc = -1;
    this->field_0xd0 = param_6;
    this->field_0xe4 = 0;
    *(void **)this = (char *)vtable + 8;
    this->field_0x4 = -1;

    int gate = 0x37a9;
    if (param_3 != 0x37a9)
        gate = 0x37a7;
    if (param_3 == 0x37a9 || param_3 == gate) {
        void *geom = operator_new(0xc0);
        uint16_t mesh = 0x37aa;
        void **holder = RocketGun_canvas_holder;
        if (param_1 == 0x37a7)
            mesh = 0x37a8;
        AEGeometry_ctor(geom, mesh, *holder, false);
        PaintCanvas_TransformAddChild(*holder, this->field_0x10,
                                      F<uint32_t>(geom, 0xc));
        operator_delete(AEGeometry_dtor(geom));
    }
}

// ---- setRadar_15ea38.cpp ----
void RocketGun::setRadar(Radar *radar)
{
    this->field_0xb0 = radar;
    void *radarObj = radar->field_0x0;
    Gun *gun = this->field_0x8;
    this->field_0xe4 = F<int>(radarObj, 0x80);

    int gunType = gun->field_0x58;
    uint32_t mode = (uint32_t)(gunType - 0x1c);
    if (mode > 2) {
        if (gunType != 0xc1) {
            int altType = gun->field_0x5c;
            if (altType != 0x28)
                goto non_special;
        }
    }
    if (gun->field_0x5c == 0x28) {
        mode = 0;
    } else if (gunType == 0xc1) {
        mode = 3;
    }

    {
        void *matrices = operator_new(0xc);
        Array_Matrix_ctor(matrices);
        this->field_0xd8 = matrices;
        void *systems = operator_new(0xc);
        Array_int_ctor(systems);
        this->field_0xdc = systems;
        void *timers = operator_new(0xc);
        Array_int_ctor(timers);
        this->field_0xe0 = timers;

        uint32_t count = F<uint32_t>(this->field_0x8, 0x8);
        ArraySetLength_Matrix(count, this->field_0xd8);
        ArraySetLength_int(F<uint32_t>(this->field_0x8, 0x8), this->field_0xdc);
        ArraySetLength_int(F<uint32_t>(this->field_0x8, 0x8), this->field_0xe0);

        int defaultType = 0x1c;
        if (mode == 2)
            defaultType = 0x1b;
        uint32_t zero = 0;
        int matrixOff = 0;
        for (uint32_t i = 0; i < F<uint32_t>(this->field_0x8, 0x8); i++) {
            int manager;
            if (F<int>(this->field_0x8, 0x58) == 0xc1) {
                manager = F<int>(radar->field_0x0, 0x98);
                this->field_0xe4 = manager;
            } else {
                manager = this->field_0xe4;
            }

            int effect;
            if (mode == 0) {
                effect = 0x19;
            } else {
                effect = defaultType;
                if (mode == 1)
                    effect = 0x1a;
            }

            int system = ParticleSystemManager_addSystem(
                manager, F<int>(this->field_0xd8, 0x4) + matrixOff, effect, false);
            *(int *)(F<char *>(this->field_0xdc, 0x4) + i * 4) = system;
            ParticleSystemManager_enableSystemEmit(this->field_0xe4, system, false);
            matrixOff += 0x3c;
            *(uint32_t *)(F<char *>(this->field_0xe0, 0x4) + i * 4) = zero;
        }
        return;
    }

non_special:
    int rocketKind = this->field_0xd0;
    if ((uint32_t)(rocketKind - 4) < 2 || rocketKind == 0x28) {
        void *matrices = operator_new(0xc);
        Array_Matrix_ctor(matrices);
        this->field_0xd8 = matrices;
        void *systems = operator_new(0xc);
        Array_int_ctor(systems);
        this->field_0xdc = systems;
        void *timers = operator_new(0xc);
        Array_int_ctor(timers);
        this->field_0xe0 = timers;

        ArraySetLength_Matrix(F<uint32_t>(this->field_0x8, 0x8), this->field_0xd8);
        ArraySetLength_int(F<uint32_t>(this->field_0x8, 0x8), this->field_0xdc);
        ArraySetLength_int(F<uint32_t>(this->field_0x8, 0x8), this->field_0xe0);

        uint32_t zero = 0;
        int matrixOff = 0;
        for (uint32_t i = 0; i < F<uint32_t>(this->field_0x8, 0x8); i++) {
            int system = ParticleSystemManager_addSystem(
                F<int>(radar->field_0x0, 0x80),
                F<int>(this->field_0xd8, 0x4) + matrixOff, 0x27, false);
            *(int *)(F<char *>(this->field_0xdc, 0x4) + i * 4) = system;
            ParticleSystemManager_enableSystemEmit(F<int>(radar->field_0x0, 0x80), system, false);
            matrixOff += 0x3c;
            *(uint32_t *)(F<char *>(this->field_0xe0, 0x4) + i * 4) = zero;
        }
        return;
    }

    if (gunType == 0xe8) {
        int manager = F<int>(radarObj, 0x9c);
        void **holder = RocketGun_canvas_holder2;
        int local = PaintCanvas_TransformGetLocal(*holder, this->field_0x10);
        int system = ParticleSystemManager_addSystem(manager, local, 0x2f, false);
        this->field_0xcc = system;
        ParticleSystemManager_attachSystem(F<int>(radar->field_0x0, 0x9c), system, 0);
    } else {
        int manager = F<int>(radarObj, 0x84);
        void **holder = RocketGun_canvas_holder2;
        int local = PaintCanvas_TransformGetLocal(*holder, this->field_0x10);
        int system = ParticleSystemManager_addSystem(manager, local, 0xc, false);
        this->field_0xcc = system;
        ParticleSystemManager_attachSystem(F<int>(radar->field_0x0, 0x84), system, 0);
    }
}

// ---- seekEnemy_15f44c.cpp ----
void RocketGun::seekEnemy(int unused, int index)
{
    char tmp1[12];
    char tmp0[12];
    char enemyPos[12];

    void *hasEnemies = ((Gun *)(this->field_0x8))->getEnemies();
    void *enemy;

    if (F<void *>(this->field_0x8, 0x4) == 0)
        goto fallback;
    if (((Gun *)(this->field_0x8))->isPlayerGun() != 0)
        goto fallback;
    if (F<void *>(this->field_0x8, 0x4) == 0)
        goto fallback;
    enemy = F<void *>(this->field_0x8, 0x4);
    if (F<int>(((Player *)(enemy))->getKIPlayer(), 0x38) < 0)
        goto fallback;
    if (Player_getEnemies(F<void *>(this->field_0x8, 0x4)) == 0)
        goto fallback;
    enemy = F<void *>(this->field_0x8, 0x4);
    enemy = ((Player *)(enemy))->getEnemy(F<int>(((Player *)(enemy))->getKIPlayer(), 0x38));
    goto have_enemy;

fallback:
    if (hasEnemies == 0)
        return;
    if (this->field_0xb0 == 0)
        return;
    enemy = F<void *>(this->field_0xb0, 0x4);
    if (enemy == 0)
        return;
    if (F<uint8_t>(enemy, 0x76) == 0 || F<uint8_t>(enemy, 0x74) != 0)
        return;
    if (((PlayerEgo *)(Level_getPlayer(F<void *>(this->field_0xb0, 0x0))))->isInFreeLookMode() != 0)
        return;
    enemy = F<void *>(F<void *>(this->field_0xb0, 0x4), 0x4);

have_enemy:
    if (enemy != 0) {
        Player_getPosition(enemyPos, enemy);
        Vector_sub(tmp0, enemyPos, (char *)F<void *>(this->field_0x8, 0xc) + index * 0xc);
        void (*fn)(void *, void *) = RocketGun_vector_func;
        fn(tmp1, tmp0);
        void *steer = (char *)this + 0xb4;
        Vector_assign(steer, tmp1);
        fn(tmp1, (char *)F<void *>(this->field_0x8, 0x18) + index * 0xc);
        Vector_isub(steer, tmp1);
        Vector_idiv(steer, this->field_0xc8 * 20.0f);
        Vector_iadd(tmp1, steer);
        fn(tmp0, tmp1);
        Vector_assign((char *)F<void *>(this->field_0x8, 0x18) + index * 0xc, tmp0);
        Vector_imul((char *)F<void *>(this->field_0x8, 0x18) + index * 0xc,
                    F<float>(this->field_0x8, 0x50));
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

    ((Gun *)(this->field_0x8))->update(elapsed);

    if (this->field_0x1c != 0 && F<uint8_t>(this->field_0x8, 0xa9) != 0) {
        void *player = Level_getPlayer(this->field_0xc);
        ((PlayerEgo *)(matrix))->getPosition();
        *(uint32_t *)gunVec = F<uint32_t>(this->field_0x8, 0x7c);
        *(uint32_t *)(gunVec + 4) = F<uint32_t>(this->field_0x8, 0x80);
        *(float *)(gunVec + 8) = F<float>(this->field_0x8, 0x84) + kMuzzleZAdd;
        MatrixRotateVector(axis, (char *)F<void *>(player, 0x0) + 4, gunVec);
        Vector_iadd(matrix, axis);
        AEGeometry_setPosition(this->field_0x18, matrix);

        float scaling = (float)F<int>(this->field_0x8, 0x70);
        scaling = (float)(int)((scaling / kScaleDiv + 1.0f) * kScaleMul);
        scaling = scaling / kScaleMul;
        AEGeometry_setScaling(this->field_0x18, scaling, scaling, scaling);

        MatrixGetDir(axis, (char *)F<void *>(player, 0x0) + 4);
        zero.x = 0.0f;
        zero.y = 1.0f;
        zero.z = 0.0f;
        AEGeometry_setDirection(this->field_0x18, axis, &zero);
    }

    Gun *gun = this->field_0x8;
    this->field_0x1d = gun->field_0xa9;

    if (gun->field_0x4d != 0) {
        this->field_0xc4 = -1;
        this->field_0xd4 = 0;
        gun->field_0x4d = 0;

        void **holder = RocketGun_canvas_holder3;
        int local = PaintCanvas_TransformGetLocal(*holder, this->field_0x10);
        int shot = F<int>(this->field_0x8, 0xa0);
        char *basePos = (char *)F<void *>(this->field_0x8, 0xc) + shot * 0xc;
        MatrixSetTranslation(matrix, *(float *)basePos, *(float *)(basePos + 4), *(float *)(basePos + 8));

        char *vel = (char *)F<void *>(this->field_0x8, 0x18) + shot * 0xc;
        *(uint32_t *)gunVec = *(uint32_t *)vel;
        *(uint32_t *)(gunVec + 4) = *(uint32_t *)(vel + 4);
        *(uint32_t *)(gunVec + 8) = *(uint32_t *)(vel + 8);

        local = PaintCanvas_TransformGetLocal(*holder, this->field_0x10);
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
        PaintCanvas_TransformSetLocal(*holder, this->field_0x10, matrix);

        if (this->field_0xd8 == 0) {
            int kind = this->field_0xd0;
            if ((uint32_t)(kind - 4) < 2 || kind == 0x28) {
                int manager = F<int>(F<void *>(this->field_0xb0, 0x0), 0x80);
                ParticleSystemManager_enableSystemRender(manager, this->field_0xcc, true);
                ParticleSystemManager_enableSystemEmit(manager, this->field_0xcc, true);
            } else {
                int manager;
                if (F<int>(this->field_0x8, 0x58) == 0xe8)
                    manager = F<int>(F<void *>(this->field_0xb0, 0x0), 0x9c);
                else
                    manager = F<int>(F<void *>(this->field_0xb0, 0x0), 0x84);
                ParticleSystemManager_enableSystemEmit(manager, this->field_0xcc,
                                                       this->field_0xcc != 0);
            }
        } else {
            int shotIndex = F<int>(this->field_0x8, 0xa0);
            char *shotPos = (char *)F<void *>(this->field_0x8, 0xc) + shotIndex * 0xc;
            int system = *(int *)(F<char *>(this->field_0xdc, 0x4) + shotIndex * 4);
            if (*(float *)(shotPos + 8) != kZeroCompare) {
                Matrix_assign(F<char *>(this->field_0xd8, 0x4) + shotIndex * 0x3c, matrix);
                ParticleSystemManager_resetSystem(this->field_0xe4, system);
                ParticleSystemManager_enableSystemEmit(this->field_0xe4, system, true);
                ParticleSystemManager_enableSystemRender(this->field_0xe4, system, true);
                *(int *)(F<char *>(this->field_0xe0, 0x4) + shotIndex * 4) = 0;
            } else {
                ParticleSystemManager_enableSystemEmit(this->field_0xe4, system, false);
                ParticleSystemManager_resetSystem(this->field_0xe4, system);
                *(int *)(F<char *>(this->field_0xe0, 0x4) +
                         F<int>(this->field_0x8, 0xa0) * 4) = 0;
            }
        }

        gun = this->field_0x8;
        if (gun->field_0x5c == 0x28 && gun->field_0xa0 > 0) {
            gun->field_0x4d = 1;
            gun->field_0xa0 = gun->field_0xa0 - 1;
            typedef void (*Fn)(RocketGun *, int);
            Fn fn = *(Fn *)(*(char **)this + 0x10);
            fn(this, elapsed);
            ((Gun *)(this->field_0x8))->update(elapsed);
            gun = this->field_0x8;
        }
    }

    if (gun->field_0x4c != 0 && gun->field_0x8 == 1 &&
        *(float *)((char *)gun->field_0xc + 8) == kZeroCompare) {
        gun->field_0x4c = 0;
        int kind = this->field_0xd0;
        int manager;
        if ((uint32_t)(kind - 4) < 2 || kind == 0x28) {
            if (this->field_0xd4 == 0)
                this->field_0xd4 = 2000;
            manager = F<int>(F<void *>(this->field_0xb0, 0x0), 0x80);
        } else {
            if (gun->field_0x58 == 0xe8)
                manager = F<int>(F<void *>(this->field_0xb0, 0x0), 0x9c);
            else
                manager = F<int>(F<void *>(this->field_0xb0, 0x0), 0x84);
        }
        ParticleSystemManager_enableSystemEmit(manager, this->field_0xcc, false);
    } else if (this->field_0xd4 > 0 && gun->field_0x8 == 1 &&
               *(float *)((char *)gun->field_0xc + 8) == kZeroCompare) {
        int timer = this->field_0xd4 - elapsed;
        this->field_0xd4 = timer;
        if (timer < 1) {
            this->field_0xd4 = 0;
            ParticleSystemManager_enableSystemRender(this->field_0xe4, this->field_0xcc, false);
        }
    }

    float dt = (float)elapsed;
    for (uint32_t i = 0, vecOff = 0, matOff = 0; i < F<uint32_t>(this->field_0x8, 0x8);
         i++, vecOff += 0xc, matOff += 0x3c) {
        gun = this->field_0x8;
        Vector_scale(axis, (char *)gun->field_0x18 + vecOff, dt);
        Vector_scale(gunVec, axis, 0.5f);
        Vector_add(matrix, (char *)gun->field_0xc + vecOff, gunVec);
        Vector_assign(&zero, matrix);

        gun = this->field_0x8;
        if (gun->field_0x4c != 0) {
            if (zero.z != kZeroCompare) {
                if (this->field_0xc0 != 0 &&
                    (this->field_0xd8 != 0 ||
                     *(int *)((char *)gun->field_0x3c + i * 4) < gun->field_0x44 - 1000)) {
                    this->seekEnemy(*(int *)((char *)gun->field_0x3c + i * 4), i);
                    gun = this->field_0x8;
                }

                if (gun->field_0x5c == 0x28) {
                    uint32_t total = gun->field_0x44;
                    uint32_t base = __aeabi_uidiv(total * i, gun->field_0x8);
                    float waveIn = (float)base +
                                   (float)(gun->field_0x44 -
                                           *(int *)((char *)gun->field_0x3c + i * 4));
                    float s = AEMath_Sinf(waveIn * kWave);
                    float c = AEMath_Cosf(waveIn * kWave);
                    VectorCross(gunVec, (char *)gun->field_0x18 + vecOff,
                                (char *)gun->field_0x24 + vecOff);
                    VectorNormalize(matrix, gunVec);
                    void (*scaleFn)(void *, void *, float) = RocketGun_vector_scale_func;
                    scaleFn(axis, matrix, s + s);
                    scaleFn(gunVec, axis, dt);
                    Vector_iadd((char *)F<void *>(this->field_0x8, 0xc) + vecOff, gunVec);
                    scaleFn(axis, (char *)F<void *>(this->field_0x8, 0x24) + vecOff, c + c);
                    scaleFn(gunVec, axis, dt);
                    Vector_iadd((char *)F<void *>(this->field_0x8, 0xc) + vecOff, gunVec);
                }
            } else {
                if (this->field_0xd8 != 0) {
                    int system = *(int *)(F<char *>(this->field_0xdc, 0x4) + i * 4);
                    ParticleSystemManager_enableSystemEmit(this->field_0xe4, system, false);
                    int *timer = (int *)(F<char *>(this->field_0xe0, 0x4) + i * 4);
                    if (*timer == 0)
                        *timer = 2000;
                }
            }

            if (this->field_0xd8 != 0) {
                gun = this->field_0x8;
                Vector_scale(axis, (char *)gun->field_0x18 + vecOff, dt);
                Vector_scale(gunVec, axis, 0.5f);
                Vector_add(matrix, (char *)gun->field_0xc + vecOff, gunVec);
                Vector_assign(&zero, matrix);
                MatrixSetTranslation(matrix, zero.x, zero.y, zero.z);
                VectorNormalize(gunVec, (char *)F<void *>(this->field_0x8, 0x18) + vecOff);
                char *m = F<char *>(this->field_0xd8, 0x4) + matOff;
                MatrixSetTranslation(matrix, zero.x, zero.y, zero.z);
                *(uint32_t *)(m + 0x8) = *(uint32_t *)gunVec;
                *(uint32_t *)(m + 0x18) = *(uint32_t *)(gunVec + 4);
                *(uint32_t *)(m + 0x28) = *(uint32_t *)(gunVec + 8);
            }
        }

        if (this->field_0xe0 != 0 && zero.z == kZeroCompare) {
            int *timer = (int *)(F<char *>(this->field_0xe0, 0x4) + i * 4);
            if (*timer > 0) {
                int left = *timer - elapsed;
                *timer = left;
                if (left < 1) {
                    *timer = 0;
                    ParticleSystemManager_enableSystemRender(
                        this->field_0xe4,
                        *(int *)(F<char *>(this->field_0xdc, 0x4) + i * 4), false);
                }
            }
        }
    }
}
