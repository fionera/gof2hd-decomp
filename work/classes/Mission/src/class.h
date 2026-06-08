#ifndef WORK_CLASSES_MISSION_SRC_CLASS_H
#define WORK_CLASSES_MISSION_SRC_CLASS_H

// Galaxy on Fire 2 — Mission class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: namespace). Field offsets recovered from the
// per-method target disassembly. We do NOT model a full layout: methods access
// fields via byte-offset casts from `this` (which arrives in r0 just like the
// first explicit arg of an extern "C" function).
//
// Known offsets:
//   0x00 vtable ptr        0x10 String name        0x1c String targetName
//   0x40 String targetStationName                  0x4c String targetSystemName
//   0x0c int id            0x3c int targetStation  0x58 int reward
//   0x5c bool instantAction 0x60 float distance     0x64 int campaign
//   0x68/0x6c production goods (int,int)            0x74 bool visible

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct Mission;

// AbyssEngine::String passed by value is a 12-byte aggregate. In the engine it has
// a non-trivial copy ctor/dtor, so by-value params are passed by invisible
// reference (a pointer). Model it opaquely.
struct String12 { uint32_t a, b, c; };

// Field accessor via byte offset.
template <class T>
static inline T &F(Mission *self, unsigned off) { return *(T *)((char *)self + off); }

#endif
