#ifndef GOF2_BLUEPRINT_H
#define GOF2_BLUEPRINT_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
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


struct BluePrint;
struct Item;
struct Station;



// AbyssEngine::String - 12-byte trivially-copied value aggregate.


// Field accessor via byte offset.

struct BluePrint {
    int32_t field_0x4;                  // +0x4
    uint8_t field_0x8;                  // +0x8
    int32_t field_0xc;                  // +0xc
    int32_t field_0x10;                 // +0x10
    int32_t field_0x20;                 // +0x20
    int32_t field_0x24;                 // +0x24
    int32_t field_0x28;                 // +0x28
};
#endif
