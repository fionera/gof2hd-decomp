#ifndef GOF2_CURVE_H
#define GOF2_CURVE_H
#include <cstdint>

namespace AbyssEngine {
    struct Curve {
        uint16_t count;
        uint16_t pad0x2;
        unsigned char *entries; // array of CurveKeyframe*, cast at use sites (keyframe type is file-local)
    };
}

#if __SIZEOF_POINTER__ == 4
#include <cstddef>
static_assert(sizeof(AbyssEngine::Curve) == 0x8, "Curve layout");
static_assert(offsetof(AbyssEngine::Curve, entries) == 0x4, "Curve.entries");
#endif

#endif
