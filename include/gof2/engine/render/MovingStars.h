#ifndef GOF2_MOVINGSTARS_H
#define GOF2_MOVINGSTARS_H
#include "gof2/common.h"
// Galaxy on Fire 2 -- MovingStars (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target names are top-level: "MovingStars::render()" (no AbyssEngine:: on the class).
// Vector/Matrix params are AbyssEngine::AEMath::Vector / ::Matrix.

#include <new>

extern "C" void *__aeabi_memcpy(void *dst, const void *src, uint32_t n);

struct MovingStars;

namespace AbyssEngine {
namespace AEMath {

} // namespace AEMath
} // namespace AbyssEngine

// MovingStars — animated starfield rendered through PaintCanvas billboards/transforms.
// Top-level class (not in AbyssEngine namespace). Four parallel 50-entry (200-byte) arrays
// hold the per-star billboard ids, transform handles, lifetimes and velocities.
class MovingStars {
public:
    uint32_t *billboardIds;      // +0x0  billboard ids array (50 entries)
    uint32_t *transformHandles;  // +0x4  transform handles array (50 entries)
    uint32_t textureHandle;      // +0x8  texture handle
    int      *lifeArray;         // +0xc  per-star lifetime/timer array (50 entries)
    int      *velocityArray;     // +0x10 per-star velocity array (50 entries)
    uint8_t   animResetFlag;     // +0x14 anim flag: low byte of the 0x101 anim word
    uint8_t   animActiveFlag;    // +0x15 anim flag: high byte of the 0x101 anim word
    char      field_0x16[2];     // +0x16 padding
    uint32_t  tickAccumulator;   // +0x18 tick accumulator
    char      field_0x1c[4];     // +0x1c

    MovingStars();
    ~MovingStars();
    void update(const AbyssEngine::AEMath::Matrix &m, bool flag, float param19);
    void translate(const AbyssEngine::AEMath::Vector &v);
    void render();
};
#endif
