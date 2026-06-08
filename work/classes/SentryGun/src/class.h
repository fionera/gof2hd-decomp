#ifndef WORK_CLASSES_SENTRYGUN_SRC_CLASS_H
#define WORK_CLASSES_SENTRYGUN_SRC_CLASS_H

// Galaxy on Fire 2 — SentryGun. Top-level, no namespace. Subclass of ObjectGun.
// Field offsets recovered from per-method target disassembly.
//
// Known fields (relative to SentryGun*):
//   +0x00  vtable
//   +0x08  Gun*  gun     (the ObjectGun-owned weapon, set by base ctor)
//   +0x0c  Level* level
//   +0xb0  int   cooldown  (init from gun->0x58 * 3 - 0x279)

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct SentryGun {
    // @portable-fields
    unsigned char _pad_0[8];
    void* f_8; // 0x8
    void* f_c; // 0xc
    unsigned char _pad_10[61];
    int f_4d; // 0x4d
    unsigned char _pad_51[7];
    int f_58; // 0x58
    unsigned char _pad_5c[68];
    int f_a0; // 0xa0
    unsigned char _pad_a4[12];
    int f_b0; // 0xb0
};
struct Gun;
struct Level;

// Field accessor via byte offset.
template <class T>
static inline T &F(void *self, unsigned off) { return *(T *)((char *)self + off); }

#endif
