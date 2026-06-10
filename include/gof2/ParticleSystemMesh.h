#ifndef GOF2_PARTICLESYSTEMMESH_H
#define GOF2_PARTICLESYSTEMMESH_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct ParticleSystemMesh;

namespace AbyssEngine {
struct PaintCanvas;
enum BlendMode { BlendMode_dummy = 0 };
namespace AEMath {
struct Matrix;

}
} // namespace AbyssEngine

namespace ParticleSettings {
enum ParticleSet { ParticleSet_dummy = 0 };
}

using PaintCanvas = AbyssEngine::PaintCanvas;
using BlendMode = AbyssEngine::BlendMode;
using Matrix = AbyssEngine::AEMath::Matrix;
using Vector = AbyssEngine::AEMath::Vector;
using ParticleSet = ParticleSettings::ParticleSet;

static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline uint16_t &u16(void *self, uint32_t off) { return *(uint16_t *)((char *)self + off); }
static inline uint8_t &u8(void *self, uint32_t off) { return *(uint8_t *)((char *)self + off); }
static inline float &f32(void *self, uint32_t off) { return *(float *)((char *)self + off); }
static inline void *&ptr(void *self, uint32_t off) { return *(void **)((char *)self + off); }

void *operator new(__SIZE_TYPE__ size);

struct ParticleSystemMesh {
    volatile uint8_t field_0x4;         // +0x4
    volatile uint32_t field_0x50;       // +0x50
    volatile uint32_t field_0x60;       // +0x60
    volatile uint8_t field_0x90;        // +0x90
    volatile uint32_t field_0x94;       // +0x94
};
#endif
