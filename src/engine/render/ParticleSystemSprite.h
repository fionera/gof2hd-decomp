#ifndef GOF2_PARTICLESYSTEMSPRITE_H
#define GOF2_PARTICLESYSTEMSPRITE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/render/IParticleSystem.h"
#include "engine/render/ParticleSettings.h"   // canonical ParticleSettings::ParticleSet nested type

namespace AbyssEngine {
    class PaintCanvas; // real type lives here (engine/render/PaintCanvas.h); pointer-only use
    enum BlendMode { BlendMode_dummy = 0, BlendMode_1 = 1, BlendMode_2 = 2, BlendMode_8 = 8, BlendMode_0x15 = 0x15 };
}

using ::AbyssEngine::PaintCanvas;
using AbyssEngine::BlendMode;

using ParticleSet = ParticleSettings::ParticleSet;

class ParticleSystemSprite : public IParticleSystem {
public:
    uint8_t started; // set by reset() once the system is primed
    uint32_t canvasHandle;
    uint8_t flags; // sprite flag byte (shadows IParticleSystem::flags)
    uint8_t flags2;
    char cAlphaChannelMode;
    int particleCount;
    int baseSize;
    uint32_t spriteId;
    uint32_t idOffset;
    uint8_t initialized; // set by init()
    int liveCount; // cleared by reset()
    void *spriteData; // per-particle sprite scratch array (12-byte elements)
    int *ages; // per-particle age array
    int8_t *setIndices; // per-particle set-index array
    float cachedPow;

    ParticleSystemSprite(PaintCanvas *canvas, const Matrix *matrix,
                         const Array<ParticleSettings::ParticleSet> &particleSets,
                         bool mirror, bool alphaFade);

    ~ParticleSystemSprite();

    // IParticleSystem virtual overrides.
    int init(uint32_t spriteId, uint16_t idOffset) override; // slot 0
    void reset() override; // slot 2
    void release() override; // slot 3
    int getQuadCount() override; // slot 4
    void updateSingle(int index, float dt) override; // slot 5
    void setParticle(const Vector &pos, float p2, uint32_t color, float p4, float p5,
                     float p6, float p7, bool clearColor, float p9, float p10,
                     const Vector &uv) override; // slot 6

    void setAlpha(int index, uint32_t color, float alpha);

    void updateAreaExitParticle(int index, float dt);

    void enable(bool enabled);

    static void render(PaintCanvas *canvas, uint32_t handle, uint32_t texture, BlendMode blend);

    static void render(PaintCanvas *canvas, uint32_t handle);
};
#endif
