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

struct SolarSystem;

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
