#ifndef GOF2_MOVINGSTARS_H
#define GOF2_MOVINGSTARS_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- MovingStars (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target names are top-level: "MovingStars::render()" (no AbyssEngine:: on the class).
// Vector/Matrix params are AbyssEngine::AEMath::Vector / ::Matrix.
// Field offsets recovered per-method from the target disasm; accessed via byte-offset casts.


#include <new>

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
extern "C" void *__aeabi_memcpy(void *dst, const void *src, uint32_t n);

struct MovingStars;

namespace AbyssEngine {
namespace AEMath {





} // namespace AEMath
} // namespace AbyssEngine

// Field accessors --------------------------------------------------------------



// MovingStars — animated starfield rendered through PaintCanvas billboards/transforms.
// Top-level class (not in AbyssEngine namespace). Field offsets are recovered from disasm;
// the four parallel arrays at 0x0/0x4/0xc/0x10 plus the texture handle (0x8) keep named fields,
// remaining scratch is reached through the i32/u32/u8/u16 helpers above.
struct MovingStars {
    void *field_0x0;                    // +0x0  billboard ids array
    uint32_t *field_0x4;                // +0x4  transform handles array
    uint32_t field_0x8;                 // +0x8  texture handle
    void *field_0xc;                    // +0xc  life array
    void *field_0x10;                   // +0x10 velocity array
    uint16_t field_0x14;                // +0x14 anim flags
    char field_0x16[2];                 // +0x16 padding
    uint32_t field_0x18;                // +0x18 tick accumulator
    char field_0x1c[4];                 // +0x1c

    MovingStars();
    ~MovingStars();
    void update(const AbyssEngine::AEMath::Matrix &m, bool flag, float param19);
    void translate(const AbyssEngine::AEMath::Vector &v);
    void render();
};
#endif
