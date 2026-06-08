#include "class.h"

__attribute__((visibility("hidden"))) extern char *ParticleSet_definitions;

extern "C" int AERandom_nextInt(void *self, int max);
extern "C" void *Vector_assign(void *self, char *other);
extern "C" void *Vector_mul(void *out, void *a, float scale);
extern "C" void *Vector_mul_scalar(void *out, float scale, void *a);
extern "C" void *Vector_plus_equal(void *self, void *other);
extern "C" void *Vector_minus_equal(void *self, void *other);

typedef void (*EmitParticleFn)(IParticleSystem *, Vector *, float, uint32_t, uint32_t, uint32_t, uint32_t,
                              uint32_t, int, float, float, void *);

void IParticleSystem::emitManual(Vector position, int particleSet, Vector const *velocity, float lifetime)
{
    if (particleSet != -1) {
        int current = I(this, 0x50);
        int set = ((int *)this->f_3c)[particleSet];
        ((uint8_t *)this->f_6c)[current] = (uint8_t)set;
        char *def = ParticleSet_definitions + (set + set * 4) * 32;
        ((int *)this->f_68)[current] = 0;

        uint32_t uv[4];
        uint32_t rotated[4];
        uv[0] = *(uint32_t *)(def + 0x88);
        uv[1] = *(uint32_t *)(def + 0x90);
        uv[2] = *(uint32_t *)(def + 0x8c);
        uv[3] = *(uint32_t *)(def + 0x94);
        uint32_t *uvp = uv;
        if ((U8(this, 0x37) & 0x80) != 0) {
            uvp = (uint32_t *)rotateUVs((float *)uv, current, (float *)rotated);
        }

        char randomVelocity[12];
        int spread = *(int *)(def + 0x50);
        if (spread == 0) {
            *(uint32_t *)(randomVelocity + 0) = 0;
            *(uint32_t *)(randomVelocity + 4) = 0;
            *(uint32_t *)(randomVelocity + 8) = 0;
        } else {
            int range = spread << 1;
            ((float *)randomVelocity)[0] = (float)(AERandom_nextInt((char *)this + 0x10, range) - spread);
            ((float *)randomVelocity)[1] = *(float *)(def + 0x5c) +
                                           (float)(AERandom_nextInt((char *)this + 0x10, range) - spread);
            ((float *)randomVelocity)[2] = (float)(AERandom_nextInt((char *)this + 0x10, range) - spread);
        }

        void *slot = vec_at(this->f_64, current);
        Vector_assign(slot, randomVelocity);

        if (velocity != 0) {
            float drag = *(float *)(def + 0x64);
            if (drag != 0.0f) {
                char tmp[12];
                Vector_mul(tmp, (void *)velocity, drag);
                Vector_minus_equal(slot, tmp);
            }
        }

        int posSpread = *(int *)(def + 0x48);
        if (posSpread != 0) {
            char randomPosition[12];
            ((float *)randomPosition)[0] =
                (float)(AERandom_nextInt((char *)this + 0x10, posSpread << 1) - posSpread);
            ((float *)randomPosition)[1] = 0.0f;
            ((float *)randomPosition)[2] =
                (float)(AERandom_nextInt((char *)this + 0x10, posSpread << 1) - posSpread);
            Vector_plus_equal(&position, randomPosition);
        }

        int ySpread = *(int *)(def + 0x4c);
        if (ySpread != 0) {
            position.y += (float)(AERandom_nextInt((char *)this + 0x10, ySpread << 1) - ySpread);
        }

        if (lifetime < 0.0f) {
            lifetime = *(float *)(def + 0x14);
        }

        char emitVelocity[12];
        int randomLife = *(int *)(def + 0x18);
        if (randomLife == 0) {
            float velocityScale = *(float *)(def + 0x24);
            if (velocityScale == 0.0f) {
                *(uint32_t *)(emitVelocity + 0) = 0;
                *(uint32_t *)(emitVelocity + 4) = 0;
                *(uint32_t *)(emitVelocity + 8) = 0;
            } else {
                Vector_mul_scalar(emitVelocity, velocityScale, slot);
            }

            EmitParticleFn fn = *(EmitParticleFn *)((char *)this->f_0 + 0x18);
            fn(this, &position, lifetime, *(uint32_t *)(def + 0x34), uvp[0], uvp[2], uvp[1], uvp[3],
               *(int *)(def + 0x3c) > 0, *(float *)(def + 0x1c), *(float *)(def + 0x20), emitVelocity);
        } else {
            float life = lifetime + (float)AERandom_nextInt((char *)this + 0x10, randomLife);
            float size0 = *(float *)(def + 0x1c) + (float)AERandom_nextInt((char *)this + 0x10, randomLife);
            float size1 = *(float *)(def + 0x20) + (float)AERandom_nextInt((char *)this + 0x10, randomLife);
            float velocityScale = *(float *)(def + 0x24);
            if (velocityScale == 0.0f) {
                *(uint32_t *)(emitVelocity + 0) = 0;
                *(uint32_t *)(emitVelocity + 4) = 0;
                *(uint32_t *)(emitVelocity + 8) = 0;
            } else {
                Vector_mul_scalar(emitVelocity, velocityScale, slot);
            }

            EmitParticleFn fn = *(EmitParticleFn *)((char *)this->f_0 + 0x18);
            fn(this, &position, life, *(uint32_t *)(def + 0x34), uvp[0], uvp[2], uvp[1], uvp[3],
               *(int *)(def + 0x3c) > 0, size0, size1, emitVelocity);
        }

        float drag = *(float *)(def + 0x64);
        if (drag != 0.0f) {
            char tmp[12];
            char tmp2[12];
            Vector_mul(tmp, (char *)this + 0x1c, drag);
            Vector_mul(tmp2, tmp, 2.0f);
            Vector_plus_equal(slot, tmp2);
        }

        current = I(this, 0x50) + 1;
        if (I(this, 0x48) <= current) {
            current = 0;
        }
        I(this, 0x50) = current;
    }
}
