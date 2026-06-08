#ifndef WORK_CLASSES_BLUEPRINT_SRC_CLASS_H
#define WORK_CLASSES_BLUEPRINT_SRC_CLASS_H

// Galaxy on Fire 2 - BluePrint class (item production blueprint).
// Android libgof2hdaa.so, armv7 Thumb. Top-level class (no AbyssEngine namespace),
// per each work-item Sig line. Field offsets recovered per-method from the target
// disassembly; access fields via byte-offset casts from `this`.
//
// Object layout (recovered from ctor / accessors):
//   +0x00  Array<int>* ingredients-remaining (quantity counters)
//   +0x04  int         spent value
//   +0x08  uint8_t     locked flag
//   +0x0c  int         production count
//   +0x10  int         station index (-1 if unset)
//   +0x14  String      station name (12 bytes)
//   +0x20  int         item index
//   +0x24  int         batch multiplier
//   +0x28  int         remaining batch

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct BluePrint;
struct Item;
struct Station;

template <class T>
struct Array {
    uint32_t length;   // +0x00 element count
    T *data;           // +0x04 element buffer
    uint32_t cap;      // +0x08 capacity
};

// AbyssEngine::String - 12-byte trivially-copied value aggregate.
struct String12 { uint32_t a, b, c; };

// Field accessor via byte offset.
template <class T>
static inline T &F(BluePrint *self, unsigned off) { return *(T *)((char *)self + off); }

#endif
