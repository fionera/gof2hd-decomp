#ifndef WORK_CLASSES_IMAGEFACTORY_SRC_CLASS_H
#define WORK_CLASSES_IMAGEFACTORY_SRC_CLASS_H

// Galaxy on Fire 2 -- ImageFactory (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target name is top-level: "ImageFactory::..." (class not in a namespace;
// only its argument types are, e.g. AbyssEngine::...).
// Field offsets recovered per-method from the target disassembly; accessed via byte-offset casts.
//
// Known layout (from ctor/reload/draw decompiles):
//   +0x00  Sprite*  itemSprite (operator new(0x40) Sprite)
//   +0x04  Array<ImagePart*>*  (item image2d handle, used by drawChar via this+4)
//   +0x08  int      ship image2d handle

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

void *operator new(__SIZE_TYPE__ size);
void *operator new[](__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

struct ImageFactory;

// Field accessor via byte offset.
template <class T>
static inline T &F(void *self, uint32_t off) { return *(T *)((char *)self + off); }

static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

#endif
