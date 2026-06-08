#ifndef WORK_CLASSES_GUN_SRC_CLASS_H
#define WORK_CLASSES_GUN_SRC_CLASS_H
// @portable-fields

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

struct Gun {
    int f_0; // 0x0
    int f_4; // 0x4
    int f_8; // 0x8
    int f_c; // 0xc
    unsigned char _pad_10[8];
    int f_18; // 0x18
    unsigned char _pad_1c[20];
    int f_30; // 0x30
    unsigned char _pad_34[4];
    int f_38; // 0x38
    void* f_3c; // 0x3c
    void* f_40; // 0x40
    int f_44; // 0x44
    int f_48; // 0x48
    int f_4c; // 0x4c
    int f_50; // 0x50
    int f_54; // 0x54
    int f_58; // 0x58
    int f_5c; // 0x5c
    int f_60; // 0x60
    int f_64; // 0x64
    int f_68; // 0x68
    int f_6c; // 0x6c
    int f_70; // 0x70
    int f_74; // 0x74
    int f_78; // 0x78
    int f_7c; // 0x7c
    int f_80; // 0x80
    int f_84; // 0x84
    int f_88; // 0x88
    unsigned char _pad_8c[4];
    int f_90; // 0x90
    int f_94; // 0x94
    int f_98; // 0x98
    unsigned char _pad_9c[4];
    int f_a0; // 0xa0
    int f_a4; // 0xa4
    int f_a8; // 0xa8
    void* f_ac; // 0xac
    int f_b0; // 0xb0
    void* f_b4; // 0xb4
    void* f_b8; // 0xb8
    void* f_bc; // 0xbc
    int f_c0; // 0xc0
    int f_c4; // 0xc4
    int f_c8; // 0xc8
    int f_cc; // 0xcc
    int f_d0; // 0xd0
    int f_d4; // 0xd4
    int f_d8; // 0xd8
    int f_dc; // 0xdc
    int f_e0; // 0xe0
    int f_e4; // 0xe4
    int f_e8; // 0xe8
    int f_ec; // 0xec
    int f_f0; // 0xf0
    int f_f4; // 0xf4
    int f_f8; // 0xf8
    int f_fc; // 0xfc
    int f_100; // 0x100
    unsigned char _pad_104[4];
    int f_108; // 0x108
    void* f_10c; // 0x10c
    void* f_110; // 0x110
};

// Field accessor via byte offset.
template <class T>
static inline T &F(Gun *self, unsigned off) { return *(T *)((char *)self + off); }

#endif
