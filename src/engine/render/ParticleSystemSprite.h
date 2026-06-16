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
    void* vtable;                       // +0x00
    uint8_t started;                    // +0x04  set by reset() once the system is primed
    uint32_t canvasHandle;              // +0x08
    uint8_t flags;                      // +0x34
    uint8_t flags2;                     // +0x37
    char cAlphaChannelMode;             // +0x45
    int particleCount;                  // +0x48
    int baseSize;                       // +0x50
    uint32_t spriteId;                  // +0x54
    uint32_t idOffset;                  // +0x58
    uint8_t initialized;                // +0x5c  set by init()
    int liveCount;                      // +0x60  cleared by reset()
    void* spriteData;                   // +0x64  per-particle sprite scratch array (12-byte elements)
    int* ages;                          // +0x68  per-particle age array
    int8_t* setIndices;                 // +0x6c  per-particle set-index array
    float cachedPow;                    // +0x70

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
