#include "class.h"

struct LocalRandom {
    char data[12];
};

extern "C" void *AERandom_seed_ctor(void *self, long long seed);
extern "C" int AERandom_nextInt(void *self, int max);
extern "C" void AERandom_dtor(void *self);

float *IParticleSystem::rotateUVs(float *src, int seed, float *dst)
{
    LocalRandom random;
    AERandom_seed_ctor(&random, (long long)seed);
    unsigned value = (unsigned)AERandom_nextInt(&random, 40000);
    unsigned inv = ~value;
    ((uint32_t *)dst)[0] = ((uint32_t *)src)[value & 1];
    ((uint32_t *)dst)[1] = ((uint32_t *)src)[inv & 1];
    ((uint32_t *)dst)[2] = *(uint32_t *)((char *)src + (((value & 2) << 1) | 8));
    ((uint32_t *)dst)[3] = *(uint32_t *)((char *)src + (((inv & 2) << 1) | 8));
    AERandom_dtor(&random);
    return dst;
}
