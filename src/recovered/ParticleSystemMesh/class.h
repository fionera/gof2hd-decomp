#ifndef WORK_CLASSES_PARTICLESYSTEMMESH_SRC_CLASS_H
#define WORK_CLASSES_PARTICLESYSTEMMESH_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned long long uint64_t;

struct ParticleSystemMesh;

namespace AbyssEngine {
struct PaintCanvas;
enum BlendMode { BlendMode_dummy = 0 };
namespace AEMath {
struct Matrix;
struct Vector {
    union {
        struct {
            float x;
            float y;
            float z;
        };
        float v[3];
    };

    Vector &operator=(const Vector &other);
};
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

void *operator new(uint32_t size);

#endif
