#ifndef GOF2_PARTICLESYSTEMSPRITE_H
#define GOF2_PARTICLESYSTEMSPRITE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// Galaxy on Fire 2 -- ParticleSystemSprite.
// A sprite-backed particle system: each particle is one entry in a PaintCanvas sprite
// system, animated (age/size/colour/UV-flipbook/position) every frame. The class itself
// lives in the global namespace; only the cross-class argument types (PaintCanvas,
// BlendMode) live in AbyssEngine.

class PaintCanvas;
namespace AbyssEngine {
using ::PaintCanvas;
enum BlendMode { BlendMode_dummy };
}
using AbyssEngine::BlendMode;

// ParticleSettings::ParticleSet -- a small index value used as a per-particle preset index.
struct ParticleSettings {
    enum ParticleSet { ParticleSet_dummy = 0 };
};
using ParticleSet = ParticleSettings::ParticleSet;

class ParticleSystemSprite {
public:
    void* vtable;
    uint8_t field_0x4;
    uint32_t canvasHandle;
    uint8_t flags;
    uint8_t flags2;
    char cAlphaChannelMode;
    int particleCount;
    int baseSize;
    uint32_t spriteId;
    uint32_t idOffset;
    uint8_t field_0x5c;
    int field_0x60;
    void* spriteData;                   // per-particle sprite scratch array (12-byte elements)
    int* ages;                          // per-particle age array
    int8_t* setIndices;                 // per-particle set-index array
    float cachedPow;

    ParticleSystemSprite(PaintCanvas *canvas, const Matrix *matrix, const void *particleSets,
                         bool mirror, bool alphaFade);
    ~ParticleSystemSprite();

    void reset();
    int init(uint32_t spriteId, uint16_t idOffset);
    void release();
    void updateSingle(int index, float dt);
    void setAlpha(int index, uint32_t color, float alpha);
    void updateAreaExitParticle(int index, float dt);
    void setParticle(const Vector &pos, float p2, uint32_t color, float p4, float p5,
                     float p6, float p7, bool clearColor, float p9, float p10, const Vector &uv);

    static void render(PaintCanvas *canvas, uint32_t handle, uint32_t texture, BlendMode blend);
    static void render(PaintCanvas *canvas, uint32_t handle);
};
#endif
