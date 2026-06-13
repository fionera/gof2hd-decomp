#ifndef GOF2_IPARTICLESYSTEM_H
#define GOF2_IPARTICLESYSTEM_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct PaintCanvas;
struct ParticleSet;

namespace AbyssEngine {
namespace AEMath {

} // namespace AEMath
} // namespace AbyssEngine

using Vector = AbyssEngine::AEMath::Vector;
using Matrix = AbyssEngine::AEMath::Matrix;

static inline void *&P(void *self, uint32_t off)
{
    return *(void **)((char *)self + off);
}

static inline int32_t &I(void *self, uint32_t off)
{
    return *(int32_t *)((char *)self + off);
}

static inline uint16_t &U16(void *self, uint32_t off)
{
    return *(uint16_t *)((char *)self + off);
}

static inline uint8_t &U8(void *self, uint32_t off)
{
    return *(uint8_t *)((char *)self + off);
}

static inline Vector *vec_at(void *base, int index)
{
    return (Vector *)((char *)base + index * 12);
}

class IParticleSystem {
public:
    void* vtable;                    // +0x0
    volatile uint16_t field_0x4;        // +0x4
    volatile uint8_t emitterVelocityDirty;         // +0x5
    PaintCanvas* canvas;             // +0x8
    Matrix const* matrix;           // +0x18
    void* particleSets;                   // +0x3c

    IParticleSystem(PaintCanvas *canvas, Matrix const *matrix, Array<int> const &sets,
                    bool mirror, bool alphaFade);
    ~IParticleSystem();
    void setParticleSet(int set);
    void setParticleSetIndex(uint8_t index);
    void setMatrix(Matrix const *matrix);
    void enableEmit(bool enabled);
    void enableRender(bool enabled);
    void update(int delta);
    void emit(int delta);
    void emitManual(Vector position, int particleSet, Vector const *velocity, float lifetime);
    void interpolateColor(int index, float *alpha, float *red, float *green, float *blue);
    float *rotateUVs(float *src, int seed, float *dst);
    void calcEmitterVelocity(int delta);
    void resetEmitterVelocity();
};
#endif
