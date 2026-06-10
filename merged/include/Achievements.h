#ifndef WORK_CLASSES_ACHIEVEMENTS_SRC_CLASS_H
#define WORK_CLASSES_ACHIEVEMENTS_SRC_CLASS_H

// Galaxy on Fire 2 — Achievements class. Top-level, no namespace.
// Field offsets recovered from per-method target disassembly. We do NOT model a
// full layout: methods access fields via byte-offset casts from `this`.
//
// Known layout (from ctor/init/decompiles):
//   +0x00  int*  medals      (operator new[](0xb4) -> 45 ints)
//   +0x04  int*  newMedals   (operator new[](0xb4) -> 45 ints)
//   +0x08  int   kills
//   +0x0c  int   catches
//   +0x10  int   pirateKills
//   +0x14  int   ?
//   +0x18  bool  ?
//   +0x1c  int   credits
//   +0x20  u16   (gotAllMedals/gotAllGoldMedals bytes 0x20/0x21)
//   +0x22  bool  gotAllSupernovaMedals

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct Achievements;

// Field accessor via byte offset.
template <class T>
static inline T &F(Achievements *self, unsigned off) { return *(T *)((char *)self + off); }

#endif
