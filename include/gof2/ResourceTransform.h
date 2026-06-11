#ifndef GOF2_RESOURCETRANSFORM_H
#define GOF2_RESOURCETRANSFORM_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- AbyssEngine::ResourceTransform (Android libgof2hdaa.so, armv7 Thumb).
// A Transform-derived resource that owns two heap arrays at +0x40 and +0x48.
// Field offsets recovered from the target disassembly; accessed via byte-offset casts.


void operator delete[](void *ptr) noexcept;

namespace AbyssEngine {

struct ResourceTransform;

} // namespace AbyssEngine

// Field accessor via byte offset.

class ResourceTransform {
public:
    void* field_0x40;                   // +0x40
    void* field_0x48;                   // +0x48
};
#endif
