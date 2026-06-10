#ifndef WORK_CLASSES_KEYFRAME_SRC_CLASS_H
#define WORK_CLASSES_KEYFRAME_SRC_CLASS_H

// Galaxy on Fire 2 -- AbyssEngine::KeyFrame (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target name: AbyssEngine::KeyFrame::KeyFrame().
// Field offsets recovered from the target disassembly; access via byte-offset casts.
//
// Object layout (recovered from ctor):
//   +0x00  block cleared to 0 over the first 0x48 bytes
//   +0x0c  Vector  scale-ish vector A   (default 1,1,1)
//   +0x30  Vector  scale-ish vector B   (default 1,1,1)
//   +0x48  float   weight               (default 1.0)
//   +0x50  int     0
//   +0x54  int     0
//   +0x58  int     0
//   +0x5c  int     0

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

extern "C" void *__stack_chk_guard;
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);
extern "C" void *__aeabi_memclr4(void *dst, uint32_t n);

namespace AbyssEngine {
namespace AEMath {

struct Vector {
    float x, y, z;
    Vector &operator=(const Vector &o);        // 0x0006eb3c
};

} // namespace AEMath

struct KeyFrame {
    // @portable-fields
    unsigned char _pad_0[72];
    int f_48; // 0x48
    unsigned char _pad_4c[4];
    int f_50; // 0x50
    int f_54; // 0x54
    int f_58; // 0x58
    int f_5c; // 0x5c

    KeyFrame();
};

} // namespace AbyssEngine

// Field accessors --------------------------------------------------------------
static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline uint8_t &u8(void *self, uint32_t off) { return *(uint8_t *)((char *)self + off); }
static inline float &f32(void *self, uint32_t off) { return *(float *)((char *)self + off); }

#endif
