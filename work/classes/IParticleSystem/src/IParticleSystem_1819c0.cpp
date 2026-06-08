#include "class.h"

__attribute__((visibility("hidden"))) extern void *IParticleSystem_vtable;
__attribute__((visibility("hidden"))) extern char *ParticleSet_definitions;

extern "C" void AERandom_ctor(void *self);
extern "C" void Array_ParticleSet_ctor(void *self);
extern "C" void ArraySet_ParticleSet(Array<int> const *src, void *dst);
extern "C" void *operator_new(uint32_t size);

IParticleSystem::IParticleSystem(PaintCanvas *canvas, Matrix const *matrix, Array<int> const &sets,
                                 bool mirror, bool alphaFade)
{
    F<PaintCanvas *>(this, 0x8) = canvas;
    P(this, 0x0) = (char *)IParticleSystem_vtable + 8;
    AERandom_ctor((char *)this + 0x10);

    uint32_t *zero = (uint32_t *)((char *)this + 0x1c);
    zero[0] = 0;
    zero[1] = 0;
    zero[2] = 0;
    zero[3] = 0;
    F<Matrix const *>(this, 0x18) = matrix;
    I(this, 0x2c) = 0;
    I(this, 0x30) = 0;

    Array_ParticleSet_ctor((char *)this + 0x38);
    U8(this, 0x4c) = mirror;
    U8(this, 0x45) = alphaFade;
    ArraySet_ParticleSet(&sets, (char *)this + 0x38);

    I(this, 0x50) = 0;
    I(this, 0x54) = -1;
    I(this, 0x58) = -1;
    U16(this, 0xc) = 0x101;
    U8(this, 0xe) = 1;
    I(this, 0x48) = 0;
    I(this, 0x34) = 0;

    int count = sets.length;
    int *src = sets.data;
    int firstFlags = 0;
    uint32_t maxParticles = 0;
    while (count != 0) {
        int set = *src;
        if (set != -1) {
            char *def = ParticleSet_definitions + (set + set * 4) * 32;
            uint32_t particles = *(uint32_t *)(def + 0x10);
            if ((int)maxParticles <= (int)particles) {
                maxParticles = particles;
            }
            U(this, 0x48) = maxParticles;
            if (firstFlags == 0) {
                firstFlags = *(int *)(def + 0xc);
                I(this, 0x34) = firstFlags;
            }
        }
        ++src;
        --count;
    }

    unsigned long long bytes64 = (unsigned long long)maxParticles * 4u;
    uint32_t bytes = (uint32_t)bytes64;
    I(this, 0x60) = 0;
    U8(this, 0x44) = 0;
    if ((uint32_t)(bytes64 >> 32) != 0) {
        bytes = 0xffffffffu;
    }
    P(this, 0x68) = operator_new(bytes);
    P(this, 0x6c) = operator_new(maxParticles | ((int32_t)maxParticles >> 31));

    for (int i = 0; i < (int)maxParticles; ++i) {
        ((uint8_t *)P(this, 0x6c))[i] = 200;
        maxParticles = U(this, 0x48);
    }

    U8(this, 0x5c) = 0;
    U16(this, 0x4) = 0x101;
}
