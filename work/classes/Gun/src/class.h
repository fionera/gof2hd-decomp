#ifndef WORK_CLASSES_GUN_SRC_CLASS_H
#define WORK_CLASSES_GUN_SRC_CLASS_H

// Galaxy on Fire 2 — Gun class (weapon/projectile emitter). Top-level, no namespace.
// Field offsets recovered from per-method target disassembly. We do NOT model a
// full layout: methods access fields via byte-offset casts from `this`.
//
// Known fields:
//   +0x08  uint  vertexCount
//   +0x0c  Vector* vertices (each 0xc bytes)
//   +0xb4  Player* enemies
//   +0xb8  Sparks* impact
//   +0xf0  bool  playerGun
//   +0xf8  bool  levelCollision
//   +0xf9  bool  friendGun
//   +0xfc  float errorMagnitudePercentage
//   +0x100 int   magnitude
//   +0x7c  Vector offset

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct Gun;

// Field accessor via byte offset.
template <class T>
static inline T &F(Gun *self, unsigned off) { return *(T *)((char *)self + off); }

#endif
