#ifndef WORK_CLASSES_STATUSWINDOW_SRC_CLASS_H
#define WORK_CLASSES_STATUSWINDOW_SRC_CLASS_H

// Galaxy on Fire 2 -- StatusWindow (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target name is top-level: "StatusWindow::..." (class not in a namespace;
// argument types like AbyssEngine::String / Array<...> are namespaced/templated).
// Field offsets recovered per-method from the target disassembly; accessed via byte-offset casts.
//
// Partial layout (from dtor/scroll/touch decompiles):
//   +0x04  Array<TouchButton*>*
//   +0x08  Array<TouchButton*>*
//   +0x0c  Array<ImagePart*>*
//   +0x10  Array<AbyssEngine::String*>*
//   +0x38  int   scroll range
//   +0x58  int   content height-ish (a)
//   +0x5c  int   content height-ish (b)

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

void *operator new(uint32_t size);
void *operator new[](uint32_t size);
void operator delete(void *ptr) noexcept;

struct StatusWindow;

namespace AbyssEngine {
struct String { uint32_t a, b, c; };
}
typedef AbyssEngine::String String;

// Field accessors via byte offset.
template <class T>
static inline T &F(void *self, uint32_t off) { return *(T *)((char *)self + off); }
static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline float &f32(void *self, uint32_t off) { return *(float *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

#endif
