#ifndef GOF2_IMAGEFACTORY_H
#define GOF2_IMAGEFACTORY_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- ImageFactory (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target name is top-level: "ImageFactory::..." (class not in a namespace;
// only its argument types are, e.g. AbyssEngine::...).
// Field offsets recovered per-method from the target disassembly; accessed via byte-offset casts.
//
// Known layout (from ctor/reload/draw decompiles):
//   +0x00  Sprite*  itemSprite (operator new(0x40) Sprite)
//   +0x04  Array<ImagePart*>*  (item image2d handle, used by drawChar via this+4)
//   +0x08  int      ship image2d handle


void *operator new(__SIZE_TYPE__ size);
void *operator new[](__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

struct ImageFactory;

// Field accessor via byte offset.


static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

struct ImageFactory {
    void* field_0x0;                    // +0x0
};
#endif
