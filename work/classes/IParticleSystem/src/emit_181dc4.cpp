#include "class.h"

__attribute__((visibility("hidden"))) extern char *ParticleSet_definitions;

extern "C" int AERandom_nextInt(void *self, int max);
extern "C" char *MatrixGetPosition(char *out, Matrix const *matrix);
extern "C" char *MatrixGetRight(char *out, Matrix const *matrix);
extern "C" char *MatrixGetUp(char *out, Matrix const *matrix);
extern "C" char *MatrixGetDir(char *out, Matrix const *matrix);
extern "C" void *Vector_assign(void *self, char *other);
extern "C" void *Vector_neg(void *out, void *a);
extern "C" void *Vector_sub(void *out, char *a, void *b);
extern "C" float Vector_dot(void *a, void *b);
extern "C" void *Vector_mul(void *out, void *a, float scale);
extern "C" void *Vector_div(void *out, void *a, float scale);
extern "C" void *Vector_add(void *out, void *a, void *b);
extern "C" void *Vector_mul_scalar(void *out, float scale, void *a);
extern "C" void *Vector_plus_equal(void *self, void *other);
extern "C" void *Vector_minus_equal(void *self, void *other);

typedef void (*EmitParticleFn)(IParticleSystem *, void *, float, uint32_t, uint32_t, uint32_t, uint32_t,
                              uint32_t, int, float, float, void *);
typedef void (*UpdateParticleFn)(IParticleSystem *, int, float);

static inline int float_bits(float v)
{
    union {
        float f;
        int i;
    } u;
    u.f = v;
    return u.i;
}

static inline float bits_float(int v)
{
    union {
        float f;
        int i;
    } u;
    u.i = v;
    return u.f;
}

static inline void zero_vec(char *v)
{
    *(uint32_t *)(v + 0) = 0;
    *(uint32_t *)(v + 4) = 0;
    *(uint32_t *)(v + 8) = 0;
}

void IParticleSystem::emit(int delta)
{
    if (U8(this, 0xc) == 0 || U8(this, 0xd) == 0) {
        return;
    }

    int set = ((int *)P(this, 0x3c))[U8(this, 0x44)];
    if (set == -1) {
        return;
    }

    uint32_t flags = U(this, 0x34);
    if ((flags & 0x80) == 0) {
        if ((flags & 0x100) != 0) {
            return;
        }
    } else if ((flags & 0x100) != 0 || ((int *)P(this, 0x68))[0] != -1) {
        return;
    }

    char matrixPos[12];
    char right[12];
    char up[12];
    char dir[12];
    char tmp[12];
    char tmp2[12];
    char travel[12];
    char travelDiv[12];
    char baseDelta[12];
    char uv[16];
    char rotated[16];
    char particlePos[12];
    char velocity[12];
    char emitVelocity[12];

    MatrixGetPosition(matrixPos, F<Matrix const *>(this, 0x18));
    MatrixGetRight(right, F<Matrix const *>(this, 0x18));
    if (U8(this, 0x4c) != 0) {
        Vector_neg(tmp, right);
        Vector_assign(right, tmp);
    }
    MatrixGetUp(up, F<Matrix const *>(this, 0x18));
    MatrixGetDir(dir, F<Matrix const *>(this, 0x18));

    char *def = ParticleSet_definitions + (set + set * 4) * 32;
    float speed2 = Vector_dot((char *)this + 0x1c, (char *)this + 0x1c);
    if (speed2 < (float)*(int *)(def + 0x98)) {
        return;
    }

    float fdelta = (float)delta;
    float elapsed = FL(this, 0x60) + fdelta;
    Vector_mul(travel, (char *)this + 0x1c, elapsed);
    Vector_div(travelDiv, travel, 1000.0f);
    float travelLen2 = Vector_dot(travelDiv, travelDiv);
    float invGuess = bits_float(0x5f3759df - (float_bits(travelLen2) >> 1));
    float invLen = (travelLen2 * -0.5f * invGuess * invGuess + 1.5f) * invGuess;
    float distance = 1.0f / invLen;

    int emitCount;
    if ((flags & 0x10) != 0) {
        float countf = distance / *(float *)(def + 0x2c);
        emitCount = (int)countf;
        FL(this, 0x60) = (elapsed * (countf - (float)emitCount)) / countf;
    } else if ((flags & 0x20) != 0) {
        int cycles = (int)(*(float *)(def + 0x2c) * elapsed * 0.001f);
        FL(this, 0x60) = elapsed + ((float)cycles * -1000.0f) / *(float *)(def + 0x2c);
        emitCount = cycles;
    } else {
        emitCount = *(int *)(def + 0x10);
        if ((flags & 0x40) != 0) {
            U8(this, 0xc) = 0;
        }
    }

    if (emitCount <= 0) {
        return;
    }

    Vector_sub(baseDelta, matrixPos, travelDiv);
    float spreadScale = 0.0f;
    float pathScale = 0.0f;
    if ((U8(this, 0x34) & 0xc0) == 0) {
        float y = bits_float(0x5f3759df - (float_bits(speed2) >> 1));
        pathScale = (speed2 * -0.5f * y * y + 1.5f) * y;
    }

    ((uint32_t *)uv)[0] = *(uint32_t *)(def + 0x88);
    ((uint32_t *)uv)[1] = *(uint32_t *)(def + 0x90);
    ((uint32_t *)uv)[2] = *(uint32_t *)(def + 0x8c);
    ((uint32_t *)uv)[3] = *(uint32_t *)(def + 0x94);

    uint32_t *uvp = (uint32_t *)uv;
    for (int i = 0; i < emitCount; ++i) {
        int current = I(this, 0x50);
        ((uint8_t *)P(this, 0x6c))[current] = (uint8_t)set;
        ((int *)P(this, 0x68))[current] = 0;
        if ((U8(this, 0x37) & 0x80) != 0) {
            uvp = (uint32_t *)rotateUVs((float *)uv, current, (float *)rotated);
        }

        int velSpread = *(int *)(def + 0x50);
        if (velSpread == 0) {
            zero_vec(velocity);
        } else {
            int range = velSpread << 1;
            ((float *)velocity)[0] = *(float *)(def + 0x58) +
                                     (float)(AERandom_nextInt((char *)this + 0x10, range) - velSpread);
            ((float *)velocity)[1] = *(float *)(def + 0x5c) +
                                     (float)(AERandom_nextInt((char *)this + 0x10, range) - velSpread);
            ((float *)velocity)[2] = *(float *)(def + 0x60) +
                                     (float)(AERandom_nextInt((char *)this + 0x10, range) - velSpread);
        }

        void *slot = vec_at(P(this, 0x64), current);
        Vector_assign(slot, velocity);

        float drag = *(float *)(def + 0x64);
        if (drag != 0.0f) {
            Vector_mul(velocity, (char *)this + 0x1c, drag);
            Vector_minus_equal(slot, velocity);
        }
        if (*(float *)(def + 0x68) != 0.0f) {
            Vector_mul(velocity, right, *(float *)(def + 0x68));
            Vector_plus_equal(slot, velocity);
        }
        if (*(float *)(def + 0x6c) != 0.0f) {
            Vector_mul(velocity, up, *(float *)(def + 0x6c));
            Vector_plus_equal(slot, velocity);
        }
        if (*(float *)(def + 0x70) != 0.0f) {
            Vector_mul(velocity, dir, *(float *)(def + 0x70));
            Vector_plus_equal(slot, velocity);
        }

        float phase;
        if (*(int *)(def + 0x30) == 1) {
            phase = (float)(i + 1);
        } else {
            phase = (float)i + (float)AERandom_nextInt((char *)this + 0x10, 10000) * 0.0001f;
        }

        zero_vec(particlePos);
        if ((U(this, 0x34) & 0xc0) == 0) {
            if (distance >= 1.0f) {
                float step = ((U(this, 0x34) & 0x10) != 0) ? *(float *)(def + 0x2c)
                                                           : distance / (float)emitCount;
                Vector_mul(tmp, travelDiv, phase * step);
                Vector_mul(tmp2, tmp, pathScale);
                Vector_assign(particlePos, tmp2);
                Vector_add(tmp2, baseDelta, particlePos);
                Vector_assign(particlePos, tmp2);
            } else {
                Vector_assign(particlePos, matrixPos);
                phase = (float)(i + 1);
                emitCount = i + 1;
            }
        } else {
            Vector_assign(particlePos, matrixPos);
            phase = 0.0f;
        }

        if ((U8(this, 0x34) & 0x80) != 0) {
            int posRange = (int)*(float *)(def + 0x78);
            int range = posRange << 1;
            ((float *)tmp)[0] = (float)(AERandom_nextInt((char *)this + 0x10, range) - posRange);
            ((float *)tmp)[1] = (float)(AERandom_nextInt((char *)this + 0x10, range) - posRange);
            ((float *)tmp)[2] = (float)(AERandom_nextInt((char *)this + 0x10, range) - posRange);
            Vector_plus_equal(particlePos, tmp);
        } else {
            if (*(float *)(def + 0x78) != 0.0f) {
                Vector_mul(tmp, right, *(float *)(def + 0x78));
                Vector_plus_equal(particlePos, tmp);
            }
            if (*(float *)(def + 0x7c) != 0.0f) {
                Vector_mul(tmp, up, *(float *)(def + 0x7c));
                Vector_plus_equal(particlePos, tmp);
            }
            if (*(float *)(def + 0x80) != 0.0f) {
                Vector_mul(tmp, dir, *(float *)(def + 0x80));
                Vector_plus_equal(particlePos, tmp);
            }
            if (*(float *)(def + 0x84) != 0.0f) {
                Vector_mul(tmp, dir, (float)AERandom_nextInt((char *)this + 0x10, (int)*(float *)(def + 0x84)));
                Vector_plus_equal(particlePos, tmp);
            }
            int posSpread = *(int *)(def + 0x48);
            if (posSpread != 0) {
                ((float *)tmp)[0] = (float)(AERandom_nextInt((char *)this + 0x10, posSpread << 1) - posSpread);
                ((float *)tmp)[1] = 0.0f;
                ((float *)tmp)[2] = (float)(AERandom_nextInt((char *)this + 0x10, posSpread << 1) - posSpread);
                Vector_plus_equal(particlePos, tmp);
            }
            int ySpread = *(int *)(def + 0x4c);
            if (ySpread != 0) {
                ((float *)particlePos)[1] +=
                    (float)(AERandom_nextInt((char *)this + 0x10, ySpread << 1) - ySpread);
            }
        }

        float life = *(float *)(def + 0x14);
        float size0 = *(float *)(def + 0x1c);
        float size1 = *(float *)(def + 0x20);
        int randomLife = *(int *)(def + 0x18);
        if (randomLife != 0) {
            life += (float)AERandom_nextInt((char *)this + 0x10, randomLife);
            size0 += (float)AERandom_nextInt((char *)this + 0x10, randomLife);
            size1 += (float)AERandom_nextInt((char *)this + 0x10, randomLife);
        }

        if (*(float *)(def + 0x24) == 0.0f) {
            zero_vec(emitVelocity);
        } else {
            Vector_mul_scalar(emitVelocity, *(float *)(def + 0x24), slot);
        }

        int colorFlag;
        if (*(int *)(def + 0x3c) > 0) {
            colorFlag = 1;
        } else {
            colorFlag = (*(float *)(def + 0x40) > 0.0f) ? 1 : 0;
        }

        EmitParticleFn emitFn = *(EmitParticleFn *)((char *)P(this, 0x0) + 0x18);
        emitFn(this, particlePos, life, *(uint32_t *)(def + 0x34), uvp[0], uvp[2], uvp[1], uvp[3],
               colorFlag, size0, size1, emitVelocity);

        if (*(float *)(def + 0x64) != 0.0f) {
            Vector_mul(tmp, (char *)this + 0x1c, *(float *)(def + 0x64));
            Vector_mul(tmp2, tmp, 2.0f);
            Vector_plus_equal(slot, tmp2);
        }

        float remaining = pathScale * invLen * ((float)emitCount - phase) * 1000.0f;
        if (remaining > fdelta) {
            remaining = fdelta;
        }
        UpdateParticleFn updateFn = *(UpdateParticleFn *)((char *)P(this, 0x0) + 0x14);
        updateFn(this, current, remaining);

        current = I(this, 0x50) + 1;
        if (I(this, 0x48) <= current) {
            current = 0;
        }
        I(this, 0x50) = current;
    }
}
