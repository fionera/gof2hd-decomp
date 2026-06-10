#ifndef GOF2_MOVINGSTARS_H
#define GOF2_MOVINGSTARS_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- MovingStars (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target names are top-level: "MovingStars::render()" (no AbyssEngine:: on the class).
// Vector/Matrix params are AbyssEngine::AEMath::Vector / ::Matrix.
// Field offsets recovered per-method from the target disasm; accessed via byte-offset casts.


void *operator new(__SIZE_TYPE__ size);
void *operator new[](__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;
void operator delete[](void *ptr) noexcept;
inline void *operator new(__SIZE_TYPE__, void *p) noexcept { return p; }

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
extern "C" void *__aeabi_memcpy(void *dst, const void *src, uint32_t n);

struct MovingStars;

namespace AbyssEngine {
namespace AEMath {





} // namespace AEMath
} // namespace AbyssEngine

// Field accessors --------------------------------------------------------------


static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline uint8_t &u8(void *self, uint32_t off) { return *(uint8_t *)((char *)self + off); }
static inline uint16_t &u16(void *self, uint32_t off) { return *(uint16_t *)((char *)self + off); }
static inline float &f32(void *self, uint32_t off) { return *(float *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

struct MovingStars {
    uint32_t* field_0x4;                // +0x4
    uint32_t field_0x8;                 // +0x8
};
#endif
