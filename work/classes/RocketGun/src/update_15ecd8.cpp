#include "class.h"

struct RocketGun {
    void update(int elapsed);
    void seekEnemy(int unused, int index);
};

extern "C" __attribute__((visibility("hidden"))) void **RocketGun_canvas_holder3;
extern "C" __attribute__((visibility("hidden"))) void (*RocketGun_vector_func2)(void *out, void *in);
extern "C" __attribute__((visibility("hidden"))) void (*RocketGun_vector_scale_func)(void *out, void *in, float scale);
extern "C" void Gun_update(void *gun, int elapsed);
extern "C" void *Level_getPlayer(void *level);
extern "C" void PlayerEgo_getPosition(void *out, void *player);
extern "C" void MatrixRotateVector(void *out, void *matrix, void *vec);
extern "C" void Vector_iadd(void *dst, void *src);
extern "C" void AEGeometry_setPosition(void *geom, void *pos);
extern "C" void AEGeometry_setScaling(void *geom, float x, float y, float z);
extern "C" void MatrixGetDir(void *out, void *matrix);
extern "C" void AEGeometry_setDirection(void *geom, void *dir, void *up);
extern "C" int PaintCanvas_TransformGetLocal(void *canvas, uint32_t id);
extern "C" void MatrixSetTranslation(void *matrix, float x, float y, float z);
extern "C" void *__aeabi_memcpy(void *dst, const void *src, uint32_t n);
extern "C" void VectorCross(void *out, void *a, void *b);
extern "C" void VectorNormalize(void *out, void *in);
extern "C" void PaintCanvas_TransformSetLocal(void *canvas, void *matrix);
extern "C" void Matrix_assign(void *dst, void *src);
extern "C" void ParticleSystemManager_enableSystemEmit(int manager, int system, bool enabled);
extern "C" void ParticleSystemManager_enableSystemRender(int manager, int system, bool enabled);
extern "C" void ParticleSystemManager_resetSystem(int manager, int system);
extern "C" uint32_t __aeabi_uidiv(uint32_t a, uint32_t b);
extern "C" float AEMath_Sinf(float v);
extern "C" float AEMath_Cosf(float v);
extern "C" void Vector_scale(void *out, void *vec, float scale);
extern "C" void Vector_add(void *out, void *a, void *b);
extern "C" void *Vector_assign(void *dst, void *src);

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

    Gun_update(F<void *>(this, 0x8), elapsed);

    if (F<uint8_t>(this, 0x1c) != 0 && F<uint8_t>(F<void *>(this, 0x8), 0xa9) != 0) {
        void *player = Level_getPlayer(F<void *>(this, 0xc));
        PlayerEgo_getPosition(matrix, player);
        *(uint32_t *)gunVec = F<uint32_t>(F<void *>(this, 0x8), 0x7c);
        *(uint32_t *)(gunVec + 4) = F<uint32_t>(F<void *>(this, 0x8), 0x80);
        *(float *)(gunVec + 8) = F<float>(F<void *>(this, 0x8), 0x84) + kMuzzleZAdd;
        MatrixRotateVector(axis, (char *)F<void *>(player, 0x0) + 4, gunVec);
        Vector_iadd(matrix, axis);
        AEGeometry_setPosition(F<void *>(this, 0x18), matrix);

        float scaling = (float)F<int>(F<void *>(this, 0x8), 0x70);
        scaling = (float)(int)((scaling / kScaleDiv + 1.0f) * kScaleMul);
        scaling = scaling / kScaleMul;
        AEGeometry_setScaling(F<void *>(this, 0x18), scaling, scaling, scaling);

        MatrixGetDir(axis, (char *)F<void *>(player, 0x0) + 4);
        zero.x = 0.0f;
        zero.y = 1.0f;
        zero.z = 0.0f;
        AEGeometry_setDirection(F<void *>(this, 0x18), axis, &zero);
    }

    void *gun = F<void *>(this, 0x8);
    F<uint8_t>(this, 0x1d) = F<uint8_t>(gun, 0xa9);

    if (F<uint8_t>(gun, 0x4d) != 0) {
        F<int>(this, 0xc4) = -1;
        F<int>(this, 0xd4) = 0;
        F<uint8_t>(gun, 0x4d) = 0;

        void **holder = RocketGun_canvas_holder3;
        int local = PaintCanvas_TransformGetLocal(*holder, F<uint32_t>(this, 0x10));
        int shot = F<int>(F<void *>(this, 0x8), 0xa0);
        char *basePos = (char *)F<void *>(F<void *>(this, 0x8), 0xc) + shot * 0xc;
        MatrixSetTranslation(matrix, *(float *)basePos, *(float *)(basePos + 4), *(float *)(basePos + 8));

        char *vel = (char *)F<void *>(F<void *>(this, 0x8), 0x18) + shot * 0xc;
        *(uint32_t *)gunVec = *(uint32_t *)vel;
        *(uint32_t *)(gunVec + 4) = *(uint32_t *)(vel + 4);
        *(uint32_t *)(gunVec + 8) = *(uint32_t *)(vel + 8);

        local = PaintCanvas_TransformGetLocal(*holder, F<uint32_t>(this, 0x10));
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
        PaintCanvas_TransformSetLocal(*holder, F<void *>(this, 0x10));

        if (F<void *>(this, 0xd8) == 0) {
            int kind = F<int>(this, 0xd0);
            if ((uint32_t)(kind - 4) < 2 || kind == 0x28) {
                int manager = F<int>(F<void *>(F<void *>(this, 0xb0), 0x0), 0x80);
                ParticleSystemManager_enableSystemRender(manager, F<int>(this, 0xcc), true);
                ParticleSystemManager_enableSystemEmit(manager, F<int>(this, 0xcc), true);
            } else {
                int manager;
                if (F<int>(F<void *>(this, 0x8), 0x58) == 0xe8)
                    manager = F<int>(F<void *>(F<void *>(this, 0xb0), 0x0), 0x9c);
                else
                    manager = F<int>(F<void *>(F<void *>(this, 0xb0), 0x0), 0x84);
                ParticleSystemManager_enableSystemEmit(manager, F<int>(this, 0xcc),
                                                       F<int>(this, 0xcc) != 0);
            }
        } else {
            int shotIndex = F<int>(F<void *>(this, 0x8), 0xa0);
            char *shotPos = (char *)F<void *>(F<void *>(this, 0x8), 0xc) + shotIndex * 0xc;
            int system = *(int *)(F<char *>(F<void *>(this, 0xdc), 0x4) + shotIndex * 4);
            if (*(float *)(shotPos + 8) != kZeroCompare) {
                Matrix_assign(F<char *>(F<void *>(this, 0xd8), 0x4) + shotIndex * 0x3c, matrix);
                ParticleSystemManager_resetSystem(F<int>(this, 0xe4), system);
                ParticleSystemManager_enableSystemEmit(F<int>(this, 0xe4), system, true);
                ParticleSystemManager_enableSystemRender(F<int>(this, 0xe4), system, true);
                *(int *)(F<char *>(F<void *>(this, 0xe0), 0x4) + shotIndex * 4) = 0;
            } else {
                ParticleSystemManager_enableSystemEmit(F<int>(this, 0xe4), system, false);
                ParticleSystemManager_resetSystem(F<int>(this, 0xe4), system);
                *(int *)(F<char *>(F<void *>(this, 0xe0), 0x4) +
                         F<int>(F<void *>(this, 0x8), 0xa0) * 4) = 0;
            }
        }

        gun = F<void *>(this, 0x8);
        if (F<int>(gun, 0x5c) == 0x28 && F<int>(gun, 0xa0) > 0) {
            F<uint8_t>(gun, 0x4d) = 1;
            F<int>(gun, 0xa0) = F<int>(gun, 0xa0) - 1;
            typedef void (*Fn)(RocketGun *, int);
            Fn fn = *(Fn *)(*(char **)this + 0x10);
            fn(this, elapsed);
            Gun_update(F<void *>(this, 0x8), elapsed);
            gun = F<void *>(this, 0x8);
        }
    }

    if (F<uint8_t>(gun, 0x4c) != 0 && F<int>(gun, 0x8) == 1 &&
        *(float *)((char *)F<void *>(gun, 0xc) + 8) == kZeroCompare) {
        F<uint8_t>(gun, 0x4c) = 0;
        int kind = F<int>(this, 0xd0);
        int manager;
        if ((uint32_t)(kind - 4) < 2 || kind == 0x28) {
            if (F<int>(this, 0xd4) == 0)
                F<int>(this, 0xd4) = 2000;
            manager = F<int>(F<void *>(F<void *>(this, 0xb0), 0x0), 0x80);
        } else {
            if (F<int>(gun, 0x58) == 0xe8)
                manager = F<int>(F<void *>(F<void *>(this, 0xb0), 0x0), 0x9c);
            else
                manager = F<int>(F<void *>(F<void *>(this, 0xb0), 0x0), 0x84);
        }
        ParticleSystemManager_enableSystemEmit(manager, F<int>(this, 0xcc), false);
    } else if (F<int>(this, 0xd4) > 0 && F<int>(gun, 0x8) == 1 &&
               *(float *)((char *)F<void *>(gun, 0xc) + 8) == kZeroCompare) {
        int timer = F<int>(this, 0xd4) - elapsed;
        F<int>(this, 0xd4) = timer;
        if (timer < 1) {
            F<int>(this, 0xd4) = 0;
            ParticleSystemManager_enableSystemRender(F<int>(this, 0xe4), F<int>(this, 0xcc), false);
        }
    }

    float dt = (float)elapsed;
    for (uint32_t i = 0, vecOff = 0, matOff = 0; i < F<uint32_t>(F<void *>(this, 0x8), 0x8);
         i++, vecOff += 0xc, matOff += 0x3c) {
        gun = F<void *>(this, 0x8);
        Vector_scale(axis, (char *)F<void *>(gun, 0x18) + vecOff, dt);
        Vector_scale(gunVec, axis, 0.5f);
        Vector_add(matrix, (char *)F<void *>(gun, 0xc) + vecOff, gunVec);
        Vector_assign(&zero, matrix);

        gun = F<void *>(this, 0x8);
        if (F<uint8_t>(gun, 0x4c) != 0) {
            if (zero.z != kZeroCompare) {
                if (F<uint8_t>(this, 0xc0) != 0 &&
                    (F<void *>(this, 0xd8) != 0 ||
                     *(int *)(F<char *>(gun, 0x3c) + i * 4) < F<int>(gun, 0x44) - 1000)) {
                    this->seekEnemy(*(int *)(F<char *>(gun, 0x3c) + i * 4), i);
                    gun = F<void *>(this, 0x8);
                }

                if (F<int>(gun, 0x5c) == 0x28) {
                    uint32_t total = F<uint32_t>(gun, 0x44);
                    uint32_t base = __aeabi_uidiv(total * i, F<uint32_t>(gun, 0x8));
                    float waveIn = (float)base +
                                   (float)(F<int>(gun, 0x44) -
                                           *(int *)(F<char *>(gun, 0x3c) + i * 4));
                    float s = AEMath_Sinf(waveIn * kWave);
                    float c = AEMath_Cosf(waveIn * kWave);
                    VectorCross(gunVec, (char *)F<void *>(gun, 0x18) + vecOff,
                                (char *)F<void *>(gun, 0x24) + vecOff);
                    VectorNormalize(matrix, gunVec);
                    void (*scaleFn)(void *, void *, float) = RocketGun_vector_scale_func;
                    scaleFn(axis, matrix, s + s);
                    scaleFn(gunVec, axis, dt);
                    Vector_iadd((char *)F<void *>(F<void *>(this, 0x8), 0xc) + vecOff, gunVec);
                    scaleFn(axis, (char *)F<void *>(F<void *>(this, 0x8), 0x24) + vecOff, c + c);
                    scaleFn(gunVec, axis, dt);
                    Vector_iadd((char *)F<void *>(F<void *>(this, 0x8), 0xc) + vecOff, gunVec);
                }
            } else {
                if (F<void *>(this, 0xd8) != 0) {
                    int system = *(int *)(F<char *>(F<void *>(this, 0xdc), 0x4) + i * 4);
                    ParticleSystemManager_enableSystemEmit(F<int>(this, 0xe4), system, false);
                    int *timer = (int *)(F<char *>(F<void *>(this, 0xe0), 0x4) + i * 4);
                    if (*timer == 0)
                        *timer = 2000;
                }
            }

            if (F<void *>(this, 0xd8) != 0) {
                gun = F<void *>(this, 0x8);
                Vector_scale(axis, (char *)F<void *>(gun, 0x18) + vecOff, dt);
                Vector_scale(gunVec, axis, 0.5f);
                Vector_add(matrix, (char *)F<void *>(gun, 0xc) + vecOff, gunVec);
                Vector_assign(&zero, matrix);
                MatrixSetTranslation(matrix, zero.x, zero.y, zero.z);
                VectorNormalize(gunVec, (char *)F<void *>(F<void *>(this, 0x8), 0x18) + vecOff);
                char *m = F<char *>(F<void *>(this, 0xd8), 0x4) + matOff;
                MatrixSetTranslation(matrix, zero.x, zero.y, zero.z);
                *(uint32_t *)(m + 0x8) = *(uint32_t *)gunVec;
                *(uint32_t *)(m + 0x18) = *(uint32_t *)(gunVec + 4);
                *(uint32_t *)(m + 0x28) = *(uint32_t *)(gunVec + 8);
            }
        }

        if (F<void *>(this, 0xe0) != 0 && zero.z == kZeroCompare) {
            int *timer = (int *)(F<char *>(F<void *>(this, 0xe0), 0x4) + i * 4);
            if (*timer > 0) {
                int left = *timer - elapsed;
                *timer = left;
                if (left < 1) {
                    *timer = 0;
                    ParticleSystemManager_enableSystemRender(
                        F<int>(this, 0xe4),
                        *(int *)(F<char *>(F<void *>(this, 0xdc), 0x4) + i * 4), false);
                }
            }
        }
    }
}
