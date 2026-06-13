#ifndef GOF2_IPARTICLESYSTEM_H
#define GOF2_IPARTICLESYSTEM_H
#include "gof2/common.h"
#include "gof2/math.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct PaintCanvas;
struct ParticleSet;

namespace AbyssEngine {
namespace AEMath {

} // namespace AEMath
} // namespace AbyssEngine

using Vector = AbyssEngine::AEMath::Vector;
using Matrix = AbyssEngine::AEMath::Matrix;

class IParticleSystem {
public:
    void* vtable;                          // +0x0
    volatile uint16_t field_0x4;           // +0x4
    volatile uint8_t emitterVelocityDirty; // +0x5
    PaintCanvas* canvas;                   // +0x8
    uint8_t emitEnabled;                   // +0xc
    uint8_t renderEnabled;                 // +0xd
    uint8_t updateEnabled;                 // +0xe
    uint8_t random[8];                     // +0x10  embedded AERandom PRNG state
    Matrix const* matrix;                  // +0x18
    Vector emitterVelocity;                // +0x1c
    Vector lastEmitterPosition;            // +0x28
    int32_t field_0x2c;                    // +0x2c
    int32_t field_0x30;                    // +0x30
    uint32_t flags;                        // +0x34  (byte +0x37 read separately)
    uint8_t particleSetIndex;              // +0x44
    uint8_t alphaFade;                     // +0x45
    int32_t maxParticles;                  // +0x48
    uint8_t mirror;                        // +0x4c
    int32_t currentParticle;               // +0x50
    int32_t field_0x54;                    // +0x54
    int32_t field_0x58;                    // +0x58
    uint8_t field_0x5c;                    // +0x5c
    float emitTimer;                       // +0x60  (int-zeroed / float accumulator)
    void* particleVelocities;              // +0x64
    void* particleAges;                    // +0x68
    void* particleSetIds;                  // +0x6c
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
