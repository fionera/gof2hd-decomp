#ifndef WORK_CLASSES_STATION_SRC_CLASS_H
#define WORK_CLASSES_STATION_SRC_CLASS_H

// Galaxy on Fire 2 — Station class. Top-level, no namespace.
// Field offsets recovered from per-method target disassembly. Methods access
// fields via byte-offset casts from `this`.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct Station {
    unsigned char _pad_0[12];
    int f_c; // 0xc
    int f_10; // 0x10
    unsigned char _pad_14[4];
    int f_18; // 0x18
    int f_1c; // 0x1c
    int f_20; // 0x20
    int f_24; // 0x24
    void* f_28; // 0x28
    void* f_2c; // 0x2c
    void* f_30; // 0x30
};

// AbyssEngine::String by value: 12-byte trivially-copied aggregate.
struct String12 { uint32_t a, b, c; };

template <class T>
struct Array {
    uint32_t length;   // +0x00
    T *data;           // +0x04
    uint32_t cap;      // +0x08
};

template <class T>
static inline T &F(Station *self, unsigned off) { return *(T *)((char *)self + off); }

#endif
