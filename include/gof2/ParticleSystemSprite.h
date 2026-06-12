#ifndef GOF2_PARTICLESYSTEMSPRITE_H
#define GOF2_PARTICLESYSTEMSPRITE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- ParticleSystemSprite (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target names are top-level: "ParticleSystemSprite::..." (the class is NOT in a
// namespace; only argument types like AbyssEngine::PaintCanvas are).
// Field offsets recovered per-method from the target disassembly; accessed via byte-offset casts.


namespace AbyssEngine {
struct PaintCanvas;
enum BlendMode { BLEND_DUMMY };
}

struct ParticleSettings {
    struct ParticleSet;
};

class ParticleSystemSprite {
public:
    uint8_t field_0x4;                  // +0x4
    unsigned int canvasHandle;             // +0x8
    unsigned char flags;           // +0x34
    unsigned char flags2;           // +0x37
    char field_0x45;                    // +0x45
    int particleCount;                     // +0x48
    int baseSize;                     // +0x50
    unsigned int spriteId;            // +0x54
    unsigned int idOffset;            // +0x58
    uint8_t field_0x5c;                 // +0x5c
    int field_0x60;                     // +0x60
    void* spriteData;                   // +0x64
    int* ages;                    // +0x68  per-particle ages array
    void* setIndices;                   // +0x6c  per-particle set-index array
    float cachedPow;                   // +0x70

    ~ParticleSystemSprite();
    void reset();
    int init(unsigned int param_1, unsigned short param_2);
    void release();
    void render(void *canvas, int handle, unsigned int texture, int blend);
    void render(void *canvas, unsigned int handle);
    void updateSingle(int index, float dt);
    void setAlpha(int param_1, unsigned int param_2, float param_3);
    void updateAreaExitParticle(int param_1, float param_2);
    void setParticle(const void *pos, float p2, unsigned int color, float p4, float p5,
                     float p6, float p7, bool clearColor, float p9, float p10, const void *uv);
};
#endif
