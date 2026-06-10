#ifndef GOF2_AEGEOMETRY_H
#define GOF2_AEGEOMETRY_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- AbyssEngine AEGeometry (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target names are top-level: "AEGeometry::isVisible()" (no AbyssEngine:: on the class).
// Field offsets recovered per-method from the target disassembly; accessed via byte-offset casts.


void *operator new(__SIZE_TYPE__ size);
void *operator new[](__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;
void operator delete[](void *ptr) noexcept;
inline void *operator new(__SIZE_TYPE__, void *p) noexcept { return p; }

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
extern "C" void *__aeabi_memcpy(void *dst, const void *src, uint32_t n);

struct AEGeometry;

namespace AbyssEngine {
namespace AEMath {





// AEMath free functions (resolved blx targets in the target).
void VectorNormalize(void *out, const Vector *v);        // 0x0006ec80
Vector operator-(const Vector &a, const Vector &b);      // 0x0006ec38
Vector MatrixGetUp(const Matrix &m);                     // 0x0006f4d8
Vector MatrixGetRight(const Matrix &m);                  // 0x0006f4e4
Vector MatrixGetPosition(const Matrix &m);               // 0x0006f16c

} // namespace AEMath

struct PaintCanvas {
    // Static transform/mesh ops; first arg is the canvas pointer.
    static uint32_t TransformGetLocal(uint32_t canvas, uint32_t tf);     // 0x000720c4
    static void TransformCreate(PaintCanvas *canvas, uint32_t *out);     // 0x000720ac
    static void TransformSetLocal(PaintCanvas *canvas, uint32_t tf, AEMath::Matrix *m); // 0x000721c0
    static void TransformAddChild(PaintCanvas *canvas, uint32_t tf, uint32_t child);    // 0x000720d0
};

} // namespace AbyssEngine

// Field accessors --------------------------------------------------------------
static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline uint8_t &u8(void *self, uint32_t off) { return *(uint8_t *)((char *)self + off); }
static inline float &f32(void *self, uint32_t off) { return *(float *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

struct AEGeometry {
    uint16_t field_0x8;                 // +0x8
    V4 field_0x30;                      // +0x30
    uint16_t field_0x48;                // +0x48
    V4 field_0x54;                      // +0x54
    unsigned long long field_0x68;      // +0x68
    uint64_t field_0x70;                // +0x70
    Matrix field_0x84;                  // +0x84
};
#endif
