#ifndef WORK_CLASSES_PLAYERFIGHTER_SRC_CLASS_H
#define WORK_CLASSES_PLAYERFIGHTER_SRC_CLASS_H

// Galaxy on Fire 2 - PlayerFighter class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine namespace). Field offsets recovered per-method from
// the target disassembly; we do NOT model a full layout - access fields via byte-offset
// casts from `self`.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct PlayerFighter;

template <class T>
struct Array {
    uint32_t length;   // +0x00 element count
    T *data;           // +0x04 element buffer
    uint32_t cap;      // +0x08 capacity
};

// AbyssEngine::AEMath::Vector - 3 floats. Passed by const ref / value.
namespace AbyssEngine { namespace AEMath {
struct Vector {
    float x, y, z;
};
} }
typedef AbyssEngine::AEMath::Vector Vector;

// AbyssEngine::String - 12-byte value aggregate (text*, field, size).
namespace AbyssEngine {
struct String { uint16_t *text; uint32_t f4; uint32_t size; };
}
typedef AbyssEngine::String String;

// Field accessor via byte offset.
template <class T>
static inline T &F(PlayerFighter *self, unsigned off) { return *(T *)((char *)self + off); }

#endif
