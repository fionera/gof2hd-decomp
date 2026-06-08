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

struct StatusWindow {
    // @portable-fields
    int f_0; // 0x0
    void* f_4; // 0x4
    void* f_8; // 0x8
    void* f_c; // 0xc
    void* f_10; // 0x10
    unsigned char _pad_14[16];
    int f_24; // 0x24
    int f_28; // 0x28
    int f_2c; // 0x2c
    int f_30; // 0x30
    int f_34; // 0x34
    int f_38; // 0x38
    int f_3c; // 0x3c
    int f_40; // 0x40
    int f_44; // 0x44
    int f_48; // 0x48
    int f_4c; // 0x4c
    int f_50; // 0x50
    int f_54; // 0x54
    int f_58; // 0x58
    int f_5c; // 0x5c
    int f_60; // 0x60
    int f_64; // 0x64
    void* f_68; // 0x68
    int f_6c; // 0x6c
    int f_70; // 0x70
    int f_74; // 0x74
    int f_78; // 0x78
};

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
