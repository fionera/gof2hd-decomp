#ifndef WORK_CLASSES_GLOBALS_SRC_CLASS_H
#define WORK_CLASSES_GLOBALS_SRC_CLASS_H

// Galaxy on Fire 2 - Globals class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine namespace). Most members are static-style helpers.
// Field offsets recovered per-method from target disasm; access via byte-offset casts.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct Globals;

// AbyssEngine::String - 12-byte value type (text*, size, ...). Pass-by-value uses the
// >8-byte struct ABI (sret / on stack), matching the target.
namespace AbyssEngine {
struct String { uint32_t a, b, c; };
}
typedef AbyssEngine::String String;

template <class T>
struct Array {
    uint32_t length;   // +0x00
    T *data;           // +0x04
    uint32_t cap;      // +0x08
};

// Field accessor via byte offset (used by the few stateful methods).
template <class T>
static inline T &F(void *self, unsigned off) { return *(T *)((char *)self + off); }

#endif
