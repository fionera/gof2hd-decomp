#ifndef GOF2_TRACTORBEAM_H
#define GOF2_TRACTORBEAM_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- TractorBeam (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine namespace), per each work-item Sig line.
// Field offsets recovered per-method from the target disassembly; access fields
// via byte-offset casts from `this`.
//
// Object layout (recovered from ctor / update):
//   +0x00  Vector      working direction/offset vector (3 floats)
//   +0x0c  KIPlayer*   currently grabbed crate object (0 = none)
//   +0x10  uint8_t     active flag
//   +0x11  uint8_t     sound-playing flag
//   +0x14  AEGeometry* beam mesh geometry
//   +0x18  int         stored hitpoints snapshot


struct TractorBeam;
struct AEGeometry;
struct KIPlayer;
struct PlayerEgo;
struct Radar;
struct Level;
struct Hud;
struct PaintCanvas;

void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

namespace AbyssEngine {
namespace AEMath {



// AEMath free functions (resolved blx targets).
void VectorNormalize(void *out, const Vector *v);   // 0x0006ec80
float VectorLength(const Vector *v);                // 0x0006ec44
Vector operator-(const Vector &a, const Vector &b); // 0x0006ec38

} // namespace AEMath
} // namespace AbyssEngine

using AbyssEngine::AEMath::Vector;

// Field accessors --------------------------------------------------------------
static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline uint8_t &u8(void *self, uint32_t off) { return *(uint8_t *)((char *)self + off); }
static inline float &f32(void *self, uint32_t off) { return *(float *)((char *)self + off); }

struct TractorBeam {
    void* field_0xc;                    // +0xc
    uint16_t field_0x10;                // +0x10
    AEGeometry* field_0x14;             // +0x14
};
#endif
