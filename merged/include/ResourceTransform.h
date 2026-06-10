#ifndef WORK_CLASSES_RESOURCETRANSFORM_SRC_CLASS_H
#define WORK_CLASSES_RESOURCETRANSFORM_SRC_CLASS_H

// Galaxy on Fire 2 -- AbyssEngine::ResourceTransform (Android libgof2hdaa.so, armv7 Thumb).
// A Transform-derived resource that owns two heap arrays at +0x40 and +0x48.
// Field offsets recovered from the target disassembly; accessed via byte-offset casts.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

void operator delete[](void *ptr) noexcept;

namespace AbyssEngine {

struct ResourceTransform;

} // namespace AbyssEngine

// Field accessor via byte offset.
template <class T>
static inline T &F(void *self, uint32_t off) { return *(T *)((char *)self + off); }

#endif
