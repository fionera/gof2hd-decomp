#ifndef GOF2_ACHIEVEMENTS_H
#define GOF2_ACHIEVEMENTS_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
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


struct Achievements;

// Field accessor via byte offset.

struct Achievements {
    int* field_0x0;                     // +0x0
    int* field_0x4;                     // +0x4
    int field_0x8;                      // +0x8
    int field_0xc;                      // +0xc
    int field_0x10;                     // +0x10
    int field_0x14;                     // +0x14
    uint8_t field_0x18;                 // +0x18
    int field_0x1c;                     // +0x1c
    uint8_t field_0x20;                 // +0x20
    uint8_t field_0x21;                 // +0x21
    uint8_t field_0x22;                 // +0x22
    int field_0x24;                     // +0x24

    Achievements();
    ~Achievements();
};
#endif
