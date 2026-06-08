#ifndef WORK_CLASSES_PLAYERFIXEDOBJECT_SRC_CLASS_H
#define WORK_CLASSES_PLAYERFIXEDOBJECT_SRC_CLASS_H

// Galaxy on Fire 2 - PlayerFixedObject class. Android libgof2hdaa.so, armv7 Thumb.
// Top-level class (no AbyssEngine namespace). Field offsets recovered per-method from the
// target disassembly; we do NOT model a full layout - access fields via byte-offset casts.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

// Opaque referenced types.
struct PlayerFixedObject {
    // @portable-fields
    unsigned char _pad_0[4];
    void* f_4; // 0x4
    void* f_8; // 0x8
    unsigned char _pad_c[24];
    int f_24; // 0x24
    int f_28; // 0x28
    int f_2c; // 0x2c
    int f_30; // 0x30
    int f_34; // 0x34
    unsigned char _pad_38[9];
    int f_41; // 0x41
    unsigned char _pad_45[7];
    int f_4c; // 0x4c
    void* f_50; // 0x50
    void* f_54; // 0x54
    int f_58; // 0x58
    int f_5c; // 0x5c
    int f_60; // 0x60
    unsigned char _pad_64[20];
    void* f_78; // 0x78
    unsigned char _pad_7c[12];
    int f_88; // 0x88
    int f_8c; // 0x8c
    int f_90; // 0x90
    int f_94; // 0x94
    int f_98; // 0x98
    unsigned char _pad_9c[16];
    int f_ac; // 0xac
    unsigned char _pad_b0[40];
    int f_d8; // 0xd8
    unsigned char _pad_dc[28];
    int f_f8; // 0xf8
    unsigned char _pad_fc[5];
    int f_101; // 0x101
    unsigned char _pad_105[31];
    void* f_124; // 0x124
    void* f_128; // 0x128
    void* f_12c; // 0x12c
    int f_130; // 0x130
    int f_134; // 0x134
    unsigned char _pad_138[8];
    int f_140; // 0x140
    int f_144; // 0x144
    int f_148; // 0x148
    int f_14c; // 0x14c
    int f_150; // 0x150
    int f_154; // 0x154
    int f_158; // 0x158
    unsigned char _pad_15c[12];
    void* f_168; // 0x168
    int f_16c; // 0x16c
    int f_170; // 0x170
    int f_174; // 0x174
    int f_178; // 0x178
    int f_17c; // 0x17c
    int f_180; // 0x180
    int f_184; // 0x184
    unsigned char _pad_188[4];
    void* f_18c; // 0x18c
    int f_190; // 0x190
    int f_194; // 0x194
    int f_198; // 0x198
    int f_19c; // 0x19c
    int f_1a0; // 0x1a0
    int f_1a4; // 0x1a4
    int f_1a8; // 0x1a8
    unsigned char _pad_1ac[12];
    int f_1b8; // 0x1b8
};
struct BoundingVolume;
struct AEGeometry;
struct Player;

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
template <class T> static inline T &F(PlayerFixedObject *p, unsigned off) {
    return *(T *)((char *)p + off);
}

#endif
