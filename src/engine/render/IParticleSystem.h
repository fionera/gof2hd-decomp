#ifndef GOF2_IPARTICLESYSTEM_H
#define GOF2_IPARTICLESYSTEM_H
#include "common.h"
#include "mathtypes.h"

class PaintCanvas;

// Base particle-system: owns the per-particle buffers and emits/updates particles
// for a configured set of particle definitions. Concrete renderers override the
// emit/update/render slots through the vtable.
class IParticleSystem {
public:
    void* vtable;
    volatile uint16_t field_0x4;
    volatile uint8_t emitterVelocityDirty;
    PaintCanvas* canvas;
    uint8_t emitEnabled;
    uint8_t renderEnabled;
    uint8_t updateEnabled;
    uint8_t random[8];                     // embedded AERandom PRNG state
    Matrix const* matrix;
    Vector emitterVelocity;
    Vector lastEmitterPosition;
    int32_t field_0x2c;
    int32_t field_0x30;
    uint32_t flags;
    uint8_t particleSetIndex;
    uint8_t alphaFade;
    int32_t maxParticles;
    uint8_t mirror;
    int32_t currentParticle;
    int32_t field_0x54;
    int32_t field_0x58;
    uint8_t field_0x5c;
    float emitTimer;
    void* particleVelocities;
    void* particleAges;
    void* particleSetIds;
    Array<int>* particleSets;              // configured particle-set indices

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
