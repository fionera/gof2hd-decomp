#ifndef WORK_CLASSES_IPARTICLESYSTEM_SRC_CLASS_H
#define WORK_CLASSES_IPARTICLESYSTEM_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct PaintCanvas;
struct ParticleSet;

template <class T>
struct Array {
    uint32_t length;
    T *data;
    uint32_t capacity;
};

namespace AbyssEngine {
namespace AEMath {
struct Vector {
    float x, y, z;
};

struct Matrix {
    float m[15];
};
} // namespace AEMath
} // namespace AbyssEngine

using Vector = AbyssEngine::AEMath::Vector;
using Matrix = AbyssEngine::AEMath::Matrix;

struct IParticleSystem {
    // @portable-fields
    void* f_0; // 0x0
    unsigned char _pad_4[4];
    void* f_8; // 0x8
    unsigned char _pad_c[12];
    void* f_18; // 0x18
    unsigned char _pad_1c[32];
    void* f_3c; // 0x3c
    unsigned char _pad_40[36];
    void* f_64; // 0x64
    void* f_68; // 0x68
    void* f_6c; // 0x6c

    IParticleSystem(PaintCanvas *canvas, Matrix const *matrix, Array<int> const &sets, bool mirror, bool alphaFade);
    ~IParticleSystem();

    void setParticleSetIndex(uint8_t index);
    void setParticleSet(int set);
    void resetEmitterVelocity();
    void calcEmitterVelocity(int delta);
    void update(int delta);
    void enableEmit(bool enabled);
    void enableRender(bool enabled);
    void emit(int delta);
    float *rotateUVs(float *src, int seed, float *dst);
    void emitManual(Vector position, int particleSet, Vector const *velocity, float lifetime);
    void setMatrix(Matrix const *matrix);
    void interpolateColor(int index, float *a, float *r, float *g, float *b);
};

template <class T>
static inline T &F(void *self, uint32_t off)
{
    return *(T *)((char *)self + off);
}

template <class T>
static inline T const &F(const void *self, uint32_t off)
{
    return *(T const *)((char const *)self + off);
}

static inline void *&P(void *self, uint32_t off)
{
    return *(void **)((char *)self + off);
}

static inline int32_t &I(void *self, uint32_t off)
{
    return *(int32_t *)((char *)self + off);
}

static inline uint32_t &U(void *self, uint32_t off)
{
    return *(uint32_t *)((char *)self + off);
}

static inline uint16_t &U16(void *self, uint32_t off)
{
    return *(uint16_t *)((char *)self + off);
}

static inline uint8_t &U8(void *self, uint32_t off)
{
    return *(uint8_t *)((char *)self + off);
}

static inline float &FL(void *self, uint32_t off)
{
    return *(float *)((char *)self + off);
}

static inline Vector *vec_at(void *base, int index)
{
    return (Vector *)((char *)base + index * 12);
}

#endif
