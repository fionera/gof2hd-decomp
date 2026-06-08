#ifndef WORK_CLASSES_SOLARSYSTEM_SRC_CLASS_H
#define WORK_CLASSES_SOLARSYSTEM_SRC_CLASS_H

// Galaxy on Fire 2 — SolarSystem class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: on the class itself), though it embeds an
// AbyssEngine::String at +0x0c. Field offsets recovered per-method from the
// target disassembly; methods access fields via byte-offset casts from `this`.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct SolarSystem {
    int f_0; // 0x0
    int f_4; // 0x4
    int f_8; // 0x8
    unsigned char _pad_c[12];
    int f_18; // 0x18
    int f_1c; // 0x1c
    int f_20; // 0x20
    int f_24; // 0x24
    int f_28; // 0x28
    int f_2c; // 0x2c
    int f_30; // 0x30
    int f_34; // 0x34
    void* f_38; // 0x38
    void* f_3c; // 0x3c
    void* f_40; // 0x40
    uint8_t f_44; // 0x44
};

// AbyssEngine::String passed/returned by value is a 12-byte trivially-copied
// aggregate (text*, size, ...). Model it opaquely so the ABI matches the target.
struct String12 { uint32_t a, b, c; };

// Array<int> header layout: { length, data, cap }.
template <class T>
struct Array { uint32_t length; T *data; uint32_t cap; };

// Field accessors via byte offset from `this`.
template <class T>
static inline T &F(SolarSystem *self, unsigned off) { return *(T *)((char *)self + off); }
static inline int32_t &i32(void *self, unsigned off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, unsigned off) { return *(uint32_t *)((char *)self + off); }
static inline uint8_t &u8(void *self, unsigned off) { return *(uint8_t *)((char *)self + off); }

#endif
