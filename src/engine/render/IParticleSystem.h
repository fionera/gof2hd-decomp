#ifndef GOF2_IPARTICLESYSTEM_H
#define GOF2_IPARTICLESYSTEM_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "mathtypes.h"
#include "engine/render/ParticleSettings.h"

namespace AbyssEngine {
    class PaintCanvas;
}

using ::AbyssEngine::PaintCanvas;

extern "C" void AERandom_dtor(void *self);

class IParticleSystem {
public:
    // Real C++ vptr lives at offset 0 (replaces the former manual `void* vtable`).
    volatile uint16_t field_0x4;
    volatile uint8_t emitterVelocityDirty;
    PaintCanvas *canvas;
    uint8_t emitEnabled;
    uint8_t renderEnabled;
    uint8_t updateEnabled;
    uint8_t random[8]; // embedded AERandom PRNG state
    Matrix const *matrix;
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
    Vector *particleVelocities; // per-particle velocity buffer (one Vector per particle)
    int *particleAges; // per-particle age in ms, -1 == slot free
    int8_t *particleSetIds; // per-particle source particle-set index
    Array<ParticleSettings::ParticleSet> *particleSets; // configured particle-set indices

    IParticleSystem(PaintCanvas *canvas, Matrix const *matrix,
                    Array<ParticleSettings::ParticleSet> const &sets,
                    bool mirror, bool alphaFade);

    // Trivial base ctor for the concrete renderers, whose construction is driven by the binary
    // base-ctor helper (_psm_base_ctor / _pss_base_ctor) rather than this C++ ctor.
    IParticleSystem() {
    }

    // Defined inline so the (abstract) class emits only the weak base-object dtor D2 the binary
    // ships -- the complete-object D1/deleting D0 variants are never needed and never emitted.
    ~IParticleSystem() {
        delete this->particleSets;
        AERandom_dtor(reinterpret_cast<void *>(this->random));
    }

    // Renderer dispatch table. These are the real C++ virtuals that replace the manual
    // vtable slots; ParticleSystemMesh / ParticleSystemSprite override them. The binary's base
    // slots are __cxa_pure_virtual (IParticleSystem is abstract and never instantiated on its
    // own), so every hook below except emit() is pure virtual. emit() carries a real base
    // implementation in the shipped binary.
    virtual int init(uint32_t resource, uint16_t idOffset) = 0; // slot 0
    virtual void emit(int delta); // slot 1 (base impl)
    virtual void reset() = 0; // slot 2
    virtual void release() = 0; // slot 3
    virtual int getQuadCount() = 0; // slot 4
    virtual void updateSingle(int index, float delta) = 0; // slot 5
    virtual void setParticle(Vector const &pos, float scale, uint32_t color,
                             float u0, float u1, float v0, float v1, bool maskedColor,
                             float size0, float size1, Vector const &velocity) = 0; // slot 6

    int getParticleCount();

    void setParticleSet(ParticleSettings::ParticleSet set);

    void setParticleSetIndex(uint8_t index);

    void setMatrix(Matrix const *matrix);

    void enableEmit(bool enabled);

    void enableRender(bool enabled);

    void enableUpdate(bool enabled);

    void update(int delta);

    void emitManual(Vector position, int particleSet, Vector const *velocity, float lifetime);

    void interpolateColor(int index, float &alpha, float &red, float &green, float &blue);

    float *rotateUVs(float *src, int seed, float *dst);

    void calcEmitterVelocity(int delta);

    void resetEmitterVelocity();
};
#endif
