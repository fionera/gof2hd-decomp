#ifndef WORK_CLASSES_MGAME_SRC_CLASS_H
#define WORK_CLASSES_MGAME_SRC_CLASS_H

// Galaxy on Fire 2 — MGame class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: namespace). Field offsets recovered from each
// per-method target disassembly. We do NOT model a full layout: methods access
// fields via byte-offset casts from `this` (which arrives in r0 just like the first
// explicit arg of an extern "C" function).
//
// The verify harness (tools/gofdiff.py) matches the built symbol by *substring* of
// the demangled name, so each method is authored as an extern "C" function whose
// name contains the target method name (e.g. MGame_OnResume). This sidesteps C++
// mangling while still producing the exact target code.
//
// Known field offsets (from per-method disasm):
//   0x00 vtable ptr
//   0x58 PlayerEgo*       0x5d  (byte) some pause flag source   0x64 String
//   0x78 Level*           0x1a6 (byte) paused flag
//   0x178 maneuver timer(int)  0x17c maneuver-active(bool)  0x180 maneuver startX
//   0x184 maneuver startPtr/Y

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct MGame;

// Field accessor via byte offset.
template <class T>
static inline T &F(MGame *self, unsigned off) { return *(T *)((char *)self + off); }

#endif
