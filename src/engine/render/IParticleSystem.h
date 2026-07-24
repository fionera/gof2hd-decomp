#ifndef GOF2_IPARTICLESYSTEM_H
#define GOF2_IPARTICLESYSTEM_H
#include "engine/core/Array.h"
#include "../core/AEString.h"
#include "engine/core/AERandom.h"
#include "engine/math/Matrix.h"
#include "engine/math/Vector.h"
#include "engine/render/ParticleSettings.h"

#include "engine/math/AEMath.h"

namespace AbyssEngine {
    class PaintCanvas;
}

using ::AbyssEngine::PaintCanvas;

class IParticleSystem {
public:
    // Set to 1 by the ctor and ParticleSystemSprite::reset, cleared by setMatrix /
    // resetEmitterVelocity; the manager polls it to decide when to re-init a system.
    uint8_t field_0x4;
    uint8_t emitterVelocityDirty;
    PaintCanvas *canvas;
    uint8_t emitEnabled;
    uint8_t renderEnabled;
    uint8_t updateEnabled;
    AbyssEngine::AERandom random;
    AbyssEngine::AEMath::Matrix const *matrix;
    AbyssEngine::AEMath::Vector emitterVelocity;
    AbyssEngine::AEMath::Vector lastEmitterPosition;
    uint32_t flags;
    Array<ParticleSettings::ParticleSet> particleSets;
    uint8_t particleSetIndex;
    uint8_t alphaFade;
    int32_t maxParticles;
    uint8_t mirror;
    int32_t currentParticle;
    // Renderer resource this system draws into: a mesh id for ParticleSystemMesh,
    // a sprite-system handle for ParticleSystemSprite. -1 until init().
    int32_t resource;
    int32_t idOffset;
    uint8_t initialized;
    float emitTimer;
    // Per-particle velocity for mesh systems; ParticleSystemMesh also stores its
    // trail edge vectors here, two per particle.
    AbyssEngine::AEMath::Vector *particleVelocities;
    int *particleAges;
    int8_t *particleSetIds;

    IParticleSystem(PaintCanvas *canvas, AbyssEngine::AEMath::Matrix const *matrix,
                    Array<ParticleSettings::ParticleSet> const &sets,
                    bool mirror, bool alphaFade);

    ~IParticleSystem() {
    }

    virtual void init(uint32_t resource, uint16_t idOffset) = 0;

    virtual void emit(int delta);

    virtual void reset() = 0;

    virtual void release() = 0;

    virtual int getQuadCount() = 0;

    virtual void updateSingle(int index, float delta) = 0;

    virtual void setParticle(AbyssEngine::AEMath::Vector const &pos, float scale, uint32_t color,
                             float u0, float u1, float v0, float v1, bool maskedColor,
                             float size0, float size1, AbyssEngine::AEMath::Vector const &velocity) = 0;

    int getParticleCount();

    void setParticleSet(ParticleSettings::ParticleSet set);

    void setParticleSetIndex(uint8_t index);

    void setMatrix(AbyssEngine::AEMath::Matrix const *matrix);

    void enableEmit(bool enabled);

    void enableRender(bool enabled);

    void enableUpdate(bool enabled);

    void update(int delta);

    void emitManual(AbyssEngine::AEMath::Vector position, int particleSet, AbyssEngine::AEMath::Vector const *velocity,
                    float lifetime);

    void interpolateColor(int index, float &alpha, float &red, float &green, float &blue);

    float *rotateUVs(float *src, int seed, float *dst);

    void calcEmitterVelocity(int delta);

    void resetEmitterVelocity();
};

#if __SIZEOF_POINTER__ == 4
static_assert(__builtin_offsetof(IParticleSystem, field_0x4) == 0x4, "IParticleSystem::field_0x4 offset");
static_assert(__builtin_offsetof(IParticleSystem, emitterVelocityDirty) == 0x5, "IParticleSystem::emitterVelocityDirty offset");
static_assert(__builtin_offsetof(IParticleSystem, canvas) == 0x8, "IParticleSystem::canvas offset");
static_assert(__builtin_offsetof(IParticleSystem, emitEnabled) == 0xc, "IParticleSystem::emitEnabled offset");
static_assert(__builtin_offsetof(IParticleSystem, random) == 0x10, "IParticleSystem::random offset");
static_assert(__builtin_offsetof(IParticleSystem, matrix) == 0x18, "IParticleSystem::matrix offset");
static_assert(__builtin_offsetof(IParticleSystem, emitterVelocity) == 0x1c, "IParticleSystem::emitterVelocity offset");
static_assert(__builtin_offsetof(IParticleSystem, lastEmitterPosition) == 0x28, "IParticleSystem::lastEmitterPosition offset");
static_assert(__builtin_offsetof(IParticleSystem, flags) == 0x34, "IParticleSystem::flags offset");
static_assert(__builtin_offsetof(IParticleSystem, particleSets) == 0x38, "IParticleSystem::particleSets offset");
static_assert(__builtin_offsetof(IParticleSystem, particleSetIndex) == 0x44, "IParticleSystem::particleSetIndex offset");
static_assert(__builtin_offsetof(IParticleSystem, alphaFade) == 0x45, "IParticleSystem::alphaFade offset");
static_assert(__builtin_offsetof(IParticleSystem, maxParticles) == 0x48, "IParticleSystem::maxParticles offset");
static_assert(__builtin_offsetof(IParticleSystem, mirror) == 0x4c, "IParticleSystem::mirror offset");
static_assert(__builtin_offsetof(IParticleSystem, currentParticle) == 0x50, "IParticleSystem::currentParticle offset");
static_assert(__builtin_offsetof(IParticleSystem, resource) == 0x54, "IParticleSystem::resource offset");
static_assert(__builtin_offsetof(IParticleSystem, idOffset) == 0x58, "IParticleSystem::idOffset offset");
static_assert(__builtin_offsetof(IParticleSystem, initialized) == 0x5c, "IParticleSystem::initialized offset");
static_assert(__builtin_offsetof(IParticleSystem, emitTimer) == 0x60, "IParticleSystem::emitTimer offset");
static_assert(__builtin_offsetof(IParticleSystem, particleVelocities) == 0x64, "IParticleSystem::particleVelocities offset");
static_assert(__builtin_offsetof(IParticleSystem, particleAges) == 0x68, "IParticleSystem::particleAges offset");
static_assert(__builtin_offsetof(IParticleSystem, particleSetIds) == 0x6c, "IParticleSystem::particleSetIds offset");
static_assert(sizeof(IParticleSystem) == 0x70, "IParticleSystem size");
#endif
#endif
