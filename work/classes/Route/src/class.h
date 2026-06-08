#ifndef WORK_CLASSES_ROUTE_SRC_CLASS_H
#define WORK_CLASSES_ROUTE_SRC_CLASS_H

// Galaxy on Fire 2 - Route class (waypoint navigation path).
// Android libgof2hdaa.so, armv7 Thumb. Top-level class (no AbyssEngine namespace),
// per each work-item Sig line. Field offsets recovered per-method from the target
// disassembly; access fields via byte-offset casts from `this`.
//
// Object layout (recovered):
//   +0x00  int                   current waypoint index
//   +0x04  uint8_t               looping flag
//   +0x0c  Array<Waypoint*>*     waypoints
//   +0x10  Array<KIPlayer*>*     docking targets
//   +0x14  Array<int>*           docking times

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct Route {
    // @portable-fields
    int f_0; // 0x0
    int f_4; // 0x4
    unsigned char _pad_8[4];
    void* f_c; // 0xc
    void* f_10; // 0x10
    void* f_14; // 0x14
};
struct Waypoint;
struct KIPlayer;

template <class T>
struct Array {
    uint32_t length;   // +0x00 element count
    T *data;           // +0x04 element buffer
    uint32_t cap;      // +0x08 capacity
};

// AbyssEngine::AEMath::Vector - 3 floats. Passed by const ref / value.
namespace AbyssEngine { namespace AEMath {
struct Vector { float x, y, z; };
} }
typedef AbyssEngine::AEMath::Vector Vector;

// Field accessor via byte offset.
template <class T>
static inline T &F(Route *self, unsigned off) { return *(T *)((char *)self + off); }

#endif
