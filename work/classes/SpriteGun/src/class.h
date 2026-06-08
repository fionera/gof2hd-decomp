#ifndef WORK_CLASSES_SPRITEGUN_SRC_CLASS_H
#define WORK_CLASSES_SPRITEGUN_SRC_CLASS_H

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

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct SpriteGun;
struct Array;
struct Player;
struct Gun;

// Field accessor via byte offset.
template <class T>
static inline T &F(void *self, unsigned off) { return *(T *)((char *)self + off); }

#endif
