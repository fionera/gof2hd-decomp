#ifndef GOF2_PARTICLESYSTEMSPRITE_H
#define GOF2_PARTICLESYSTEMSPRITE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/IParticleSystem.h"
#include "engine/render/ParticleSettings.h"   // canonical ParticleSettings::ParticleSet nested type

// Galaxy on Fire 2 -- ParticleSystemSprite.
// A sprite-backed particle system: each particle is one entry in a PaintCanvas sprite
// system, animated (age/size/colour/UV-flipbook/position) every frame. It is one of the two
// concrete IParticleSystem renderers; the update/render/setParticle hooks are real overrides
// of the base virtual slots.

namespace AbyssEngine {
class PaintCanvas;   // real type lives here (engine/render/PaintCanvas.h); pointer-only use
enum BlendMode { BlendMode_dummy };
}
using ::AbyssEngine::PaintCanvas;
using AbyssEngine::BlendMode;

// ParticleSettings::ParticleSet -- a small index value used as a per-particle preset index.
// The canonical definition lives in engine/render/ParticleSettings.h (included above).
using ParticleSet = ParticleSettings::ParticleSet;

// ParticleSystemSprite is a concrete IParticleSystem renderer. The base supplies the vptr and the
// shared emitter state; the members below are the sprite-specific extension fields (note `flags` is a
// byte-typed sprite flag word that deliberately shadows the base 32-bit `flags`). The
// init/reset/release/updateSingle/setParticle hooks are real overrides of the base virtual slots.
class ParticleSystemSprite : public IParticleSystem {
public:
    uint8_t started;                    // set by reset() once the system is primed
    uint32_t canvasHandle;
    uint8_t flags;                      // sprite flag byte (shadows IParticleSystem::flags)
    uint8_t flags2;
    char cAlphaChannelMode;
    int particleCount;
    int baseSize;
    uint32_t spriteId;
    uint32_t idOffset;
    uint8_t initialized;                // set by init()
    int liveCount;                      // cleared by reset()
    void* spriteData;                   // per-particle sprite scratch array (12-byte elements)
    int* ages;                          // per-particle age array
    int8_t* setIndices;                 // per-particle set-index array
    float cachedPow;

    ParticleSystemSprite(PaintCanvas *canvas, const Matrix *matrix, const void *particleSets,
                         bool mirror, bool alphaFade);
    ~ParticleSystemSprite();

    // IParticleSystem virtual overrides.
    int  init(uint32_t spriteId, uint16_t idOffset) override;   // slot 0
    void reset() override;                                       // slot 2
    void release() override;                                     // slot 3
    int  getQuadCount() override;                                // slot 4
    void updateSingle(int index, float dt) override;            // slot 5
    void setParticle(const Vector &pos, float p2, uint32_t color, float p4, float p5,
                     float p6, float p7, bool clearColor, float p9, float p10,
                     const Vector &uv) override;                 // slot 6

    void setAlpha(int index, uint32_t color, float alpha);
    void updateAreaExitParticle(int index, float dt);

    static void render(PaintCanvas *canvas, uint32_t handle, uint32_t texture, BlendMode blend);
    static void render(PaintCanvas *canvas, uint32_t handle);
};
#endif
