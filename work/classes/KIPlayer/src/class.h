#ifndef WORK_CLASSES_KIPLAYER_SRC_CLASS_H
#define WORK_CLASSES_KIPLAYER_SRC_CLASS_H

// Galaxy on Fire 2 - KIPlayer class (enemy AI). Android libgof2hdaa.so, armv7 Thumb.
// Top-level class (no AbyssEngine namespace). Field offsets recovered per-method from the
// target disassembly; we do NOT model a full layout - access fields via byte-offset casts.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

// Opaque referenced types.
struct KIPlayer {
    // @portable-fields
    unsigned char _pad_0[4];
    void* f_4; // 0x4
    void* f_8; // 0x8
    void* f_c; // 0xc
    int f_10; // 0x10
    int f_14; // 0x14
    unsigned char _pad_18[12];
    int f_24; // 0x24
    int f_28; // 0x28
    int f_2c; // 0x2c
    int f_30; // 0x30
    int f_34; // 0x34
    unsigned char _pad_38[4];
    int f_3c; // 0x3c
    int f_40; // 0x40
    int f_44; // 0x44
    int f_48; // 0x48
    int f_4c; // 0x4c
    void* f_50; // 0x50
    void* f_54; // 0x54
    unsigned char _pad_58[12];
    int f_64; // 0x64
    int f_68; // 0x68
    int f_6c; // 0x6c
    int f_70; // 0x70
    unsigned char _pad_74[1];
    int f_75; // 0x75
    unsigned char _pad_79[7];
    int f_80; // 0x80
    int f_84; // 0x84
    int f_88; // 0x88
    int f_8c; // 0x8c
    int f_90; // 0x90
    int f_94; // 0x94
    int f_98; // 0x98
    int f_9c; // 0x9c
    int f_a0; // 0xa0
    int f_a4; // 0xa4
    int f_a8; // 0xa8
    int f_ac; // 0xac
    unsigned char _pad_b0[1];
    int f_b1; // 0xb1
    unsigned char _pad_b5[7];
    int f_bc; // 0xbc
    int f_c0; // 0xc0
    int f_c4; // 0xc4
    int f_c8; // 0xc8
    void* f_cc; // 0xcc
    int f_d0; // 0xd0
    unsigned char _pad_d4[4];
    int f_d8; // 0xd8
    int f_dc; // 0xdc
    unsigned char _pad_e0[8];
    int f_e8; // 0xe8
    int f_ec; // 0xec
    unsigned char _pad_f0[4];
    int f_f4; // 0xf4
    int f_f8; // 0xf8
    int f_fc; // 0xfc
    int f_100; // 0x100
    int f_104; // 0x104
    unsigned char _pad_108[4];
    int f_10c; // 0x10c
    int f_110; // 0x110
    int f_114; // 0x114
    int f_118; // 0x118
    int f_11c; // 0x11c
    int f_120; // 0x120
};
struct Route;
struct Gun;
struct SpacePoint;

template <class T>
struct Array {
    uint32_t length;   // +0x00 element count
    T *data;           // +0x04 element buffer
    uint32_t cap;      // +0x08 capacity
};

// AbyssEngine::AEMath::Vector - 3 floats. Passed by const ref / value.
namespace AbyssEngine { namespace AEMath {
struct Vector {
    float x, y, z;
};
} }
typedef AbyssEngine::AEMath::Vector Vector;

// Field accessors via byte offset.
template <class T> static inline T &F(KIPlayer *p, unsigned off) {
    return *(T *)((char *)p + off);
}

#endif
