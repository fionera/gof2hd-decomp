#include "class.h"

struct RocketGun {
    void setRadar(Radar *radar);
};

extern "C" __attribute__((visibility("hidden"))) void **RocketGun_canvas_holder2;
extern "C" void *operator_new(uint32_t size);
extern "C" void Array_Matrix_ctor(void *self);
extern "C" void Array_int_ctor(void *self);
extern "C" void ArraySetLength_Matrix(uint32_t length, void *array);
extern "C" void ArraySetLength_int(uint32_t length, void *array);
extern "C" int PaintCanvas_TransformGetLocal(void *canvas, uint32_t id);
extern "C" int ParticleSystemManager_addSystem(int manager, int transform, int type, bool flag);
extern "C" void ParticleSystemManager_enableSystemEmit(int manager, int system, bool enabled);
extern "C" void ParticleSystemManager_attachSystem(int manager, int system, int zero);

void RocketGun::setRadar(Radar *radar)
{
    F<Radar *>(this, 0xb0) = radar;
    void *radarObj = F<void *>(radar, 0x0);
    void *gun = F<void *>(this, 0x8);
    F<int>(this, 0xe4) = F<int>(radarObj, 0x80);

    int gunType = F<int>(gun, 0x58);
    uint32_t mode = (uint32_t)(gunType - 0x1c);
    if (mode > 2) {
        if (gunType != 0xc1) {
            int altType = F<int>(gun, 0x5c);
            if (altType != 0x28)
                goto non_special;
        }
    }
    if (F<int>(gun, 0x5c) == 0x28) {
        mode = 0;
    } else if (gunType == 0xc1) {
        mode = 3;
    }

    {
        void *matrices = operator_new(0xc);
        Array_Matrix_ctor(matrices);
        F<void *>(this, 0xd8) = matrices;
        void *systems = operator_new(0xc);
        Array_int_ctor(systems);
        F<void *>(this, 0xdc) = systems;
        void *timers = operator_new(0xc);
        Array_int_ctor(timers);
        F<void *>(this, 0xe0) = timers;

        uint32_t count = F<uint32_t>(F<void *>(this, 0x8), 0x8);
        ArraySetLength_Matrix(count, F<void *>(this, 0xd8));
        ArraySetLength_int(F<uint32_t>(F<void *>(this, 0x8), 0x8), F<void *>(this, 0xdc));
        ArraySetLength_int(F<uint32_t>(F<void *>(this, 0x8), 0x8), F<void *>(this, 0xe0));

        int defaultType = 0x1c;
        if (mode == 2)
            defaultType = 0x1b;
        uint32_t zero = 0;
        int matrixOff = 0;
        for (uint32_t i = 0; i < F<uint32_t>(F<void *>(this, 0x8), 0x8); i++) {
            int manager;
            if (F<int>(F<void *>(this, 0x8), 0x58) == 0xc1) {
                manager = F<int>(F<void *>(radar, 0x0), 0x98);
                F<int>(this, 0xe4) = manager;
            } else {
                manager = F<int>(this, 0xe4);
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
                manager, F<int>(F<void *>(this, 0xd8), 0x4) + matrixOff, effect, false);
            *(int *)(F<char *>(F<void *>(this, 0xdc), 0x4) + i * 4) = system;
            ParticleSystemManager_enableSystemEmit(F<int>(this, 0xe4), system, false);
            matrixOff += 0x3c;
            *(uint32_t *)(F<char *>(F<void *>(this, 0xe0), 0x4) + i * 4) = zero;
        }
        return;
    }

non_special:
    int rocketKind = F<int>(this, 0xd0);
    if ((uint32_t)(rocketKind - 4) < 2 || rocketKind == 0x28) {
        void *matrices = operator_new(0xc);
        Array_Matrix_ctor(matrices);
        F<void *>(this, 0xd8) = matrices;
        void *systems = operator_new(0xc);
        Array_int_ctor(systems);
        F<void *>(this, 0xdc) = systems;
        void *timers = operator_new(0xc);
        Array_int_ctor(timers);
        F<void *>(this, 0xe0) = timers;

        ArraySetLength_Matrix(F<uint32_t>(F<void *>(this, 0x8), 0x8), F<void *>(this, 0xd8));
        ArraySetLength_int(F<uint32_t>(F<void *>(this, 0x8), 0x8), F<void *>(this, 0xdc));
        ArraySetLength_int(F<uint32_t>(F<void *>(this, 0x8), 0x8), F<void *>(this, 0xe0));

        uint32_t zero = 0;
        int matrixOff = 0;
        for (uint32_t i = 0; i < F<uint32_t>(F<void *>(this, 0x8), 0x8); i++) {
            int system = ParticleSystemManager_addSystem(
                F<int>(F<void *>(radar, 0x0), 0x80),
                F<int>(F<void *>(this, 0xd8), 0x4) + matrixOff, 0x27, false);
            *(int *)(F<char *>(F<void *>(this, 0xdc), 0x4) + i * 4) = system;
            ParticleSystemManager_enableSystemEmit(F<int>(F<void *>(radar, 0x0), 0x80), system, false);
            matrixOff += 0x3c;
            *(uint32_t *)(F<char *>(F<void *>(this, 0xe0), 0x4) + i * 4) = zero;
        }
        return;
    }

    if (gunType == 0xe8) {
        int manager = F<int>(radarObj, 0x9c);
        void **holder = RocketGun_canvas_holder2;
        int local = PaintCanvas_TransformGetLocal(*holder, F<uint32_t>(this, 0x10));
        int system = ParticleSystemManager_addSystem(manager, local, 0x2f, false);
        F<int>(this, 0xcc) = system;
        ParticleSystemManager_attachSystem(F<int>(F<void *>(radar, 0x0), 0x9c), system, 0);
    } else {
        int manager = F<int>(radarObj, 0x84);
        void **holder = RocketGun_canvas_holder2;
        int local = PaintCanvas_TransformGetLocal(*holder, F<uint32_t>(this, 0x10));
        int system = ParticleSystemManager_addSystem(manager, local, 0xc, false);
        F<int>(this, 0xcc) = system;
        ParticleSystemManager_attachSystem(F<int>(F<void *>(radar, 0x0), 0x84), system, 0);
    }
}
