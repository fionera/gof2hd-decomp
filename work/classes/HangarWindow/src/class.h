#ifndef WORK_CLASSES_HANGARWINDOW_SRC_CLASS_H
#define WORK_CLASSES_HANGARWINDOW_SRC_CLASS_H

// Galaxy on Fire 2 - HangarWindow class. Android libgof2hdaa.so, armv7 Thumb.
// Top-level class (no AbyssEngine namespace). Field offsets recovered per-method from the
// target disassembly; we do NOT model a full layout - access fields via byte-offset casts.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

// Opaque referenced types.
struct HangarWindow {
    // @portable-fields
    int f_0; // 0x0
    void* f_4; // 0x4
    int f_8; // 0x8
    int f_c; // 0xc
    void* f_10; // 0x10
    void* f_14; // 0x14
    void* f_18; // 0x18
    void* f_1c; // 0x1c
    void* f_20; // 0x20
    void* f_24; // 0x24
    void* f_28; // 0x28
    void* f_2c; // 0x2c
    void* f_30; // 0x30
    unsigned char _pad_34[8];
    int f_3c; // 0x3c
    int f_40; // 0x40
    int f_44; // 0x44
    int f_48; // 0x48
    int f_4c; // 0x4c
    int f_50; // 0x50
    void* f_54; // 0x54
    int f_58; // 0x58
    unsigned char _pad_5c[12];
    void* f_68; // 0x68
    int f_6c; // 0x6c
    int f_70; // 0x70
    unsigned char _pad_74[4];
    int f_78; // 0x78
    int f_7c; // 0x7c
    void* f_80; // 0x80
    void* f_84; // 0x84
    int f_88; // 0x88
    int f_8c; // 0x8c
    void* f_90; // 0x90
    int f_94; // 0x94
    int f_98; // 0x98
    int f_9c; // 0x9c
    int f_a0; // 0xa0
    int f_a4; // 0xa4
    int f_a8; // 0xa8
    int f_ac; // 0xac
    int f_b0; // 0xb0
    int f_b4; // 0xb4
    int f_b8; // 0xb8
    int f_bc; // 0xbc
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
    unsigned char _pad_e8[8];
    int f_f0; // 0xf0
    int f_f4; // 0xf4
    int f_f8; // 0xf8
    int f_fc; // 0xfc
    unsigned char _pad_100[12];
    int f_10c; // 0x10c
    int f_110; // 0x110
    int f_114; // 0x114
    int f_118; // 0x118
    int f_11c; // 0x11c
    int f_120; // 0x120
    int f_124; // 0x124
    int f_128; // 0x128
    int f_12c; // 0x12c
    int f_130; // 0x130
};
struct HangarList;
struct ListItem;
struct Item;
struct BluePrint;
struct Station;

template <class T>
struct Array {
    uint32_t length;   // +0x00 element count
    T *data;           // +0x04 element buffer
    uint32_t cap;      // +0x08 capacity
};

// AbyssEngine::AEMath::Vector - 3 floats.
namespace AbyssEngine { namespace AEMath {
struct Vector { float x, y, z; };
} }
typedef AbyssEngine::AEMath::Vector Vector;

// Field accessors via byte offset.
template <class T> static inline T &F(HangarWindow *p, unsigned off) {
    return *(T *)((char *)p + off);
}
template <class T> static inline T &G(void *p, unsigned off) {
    return *(T *)((char *)p + off);
}

#endif
