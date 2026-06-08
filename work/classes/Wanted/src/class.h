#ifndef WORK_CLASSES_WANTED_SRC_CLASS_H
#define WORK_CLASSES_WANTED_SRC_CLASS_H

// Galaxy on Fire 2 — Wanted class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: namespace). Field offsets recovered from the
// per-method target disassembly. We do NOT model a full layout: methods access
// fields via byte-offset casts from `this` (which arrives in r0 just like the
// first explicit arg of an extern "C" function).

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct Wanted {
    // @portable-fields
    unsigned char _pad_0[12];
    int f_c; // 0xc
    unsigned char _pad_10[48];
    void* f_40; // 0x40
    unsigned char _pad_44[12];
    int f_50; // 0x50
};

// AbyssEngine::String passed by value is a 12-byte trivially-copied aggregate
// (text*, size, ...). Model it opaquely so it is passed the same way as the
// target ABI (struct > 8 bytes by value -> on the stack / sret-style).
struct String12 { uint32_t a, b, c; };

// Field accessor via byte offset.
template <class T>
static inline T &F(Wanted *self, unsigned off) { return *(T *)((char *)self + off); }

#endif
