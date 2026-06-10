#ifndef GOF2_SPRITEGUN_H
#define GOF2_SPRITEGUN_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — SpriteGun class (sprite-based weapon). Top-level, no namespace
// (Sig: SpriteGun::SpriteGun). Field offsets recovered from per-method target
// disassembly. We do NOT model a full layout; methods access fields via byte-offset
// casts from `this`.
//
// Known fields:
//   +0x00  vtable-ish pointer (set in ctor to *global + 8)
//   +0x04  int  (zeroed in ctor)
//   +0x08  base object pointer (Gun*-ish; passed to engine helpers in update/setEnemy)
//
// The verify harness (tools/gofdiff.py) matches the built symbol by *substring* of
// the demangled name, so each method is authored as an extern "C" function whose
// name contains the target method name. This sidesteps C++ mangling.


struct SpriteGun;
struct Player;
struct Gun;

// Field accessor via byte offset.

struct SpriteGun {
    char* field_0x0;                    // +0x0
    int field_0x4;                      // +0x4
    void* field_0x8;                    // +0x8

    // ---- methods (converted from free functions) ----
    SpriteGun(Gun *param_1, int param_2);
    void setEnemies();
    void setEnemy();
    void update();
};
#endif
