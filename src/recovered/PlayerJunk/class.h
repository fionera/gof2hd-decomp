#ifndef WORK_CLASSES_PLAYERJUNK_SRC_CLASS_H
#define WORK_CLASSES_PLAYERJUNK_SRC_CLASS_H

// Galaxy on Fire 2 - PlayerJunk class (a KIPlayer subclass: destructible space junk).
// Android libgof2hdaa.so, armv7 Thumb. Top-level class (no AbyssEngine namespace).
// Field offsets recovered per-method from the target disassembly; we do NOT model a full
// layout - methods access fields via byte-offset casts from `this`.
//
// Known field offsets:
//   0x00 vtable        0x04 int (player/owner id)   0x4c uint8 flag
//   0x50 Array<int>*   0x54 ParticleSystem owner    0x58/5c/60 Vector (emit pos)
//   0x78 int (geometry present)   0x88 int state     0x124 int (last update tick)

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct Player;
struct AEGeometry;

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

void *operator new(unsigned);

// Field accessor via byte offset.
template <class T> static inline T &F(void *p, unsigned off) {
    return *(T *)((char *)p + off);
}

#endif
