#ifndef WORK_CLASSES_STATION_SRC_CLASS_H
#define WORK_CLASSES_STATION_SRC_CLASS_H

// Galaxy on Fire 2 — Station class. Top-level, no namespace.
// Field offsets recovered from per-method target disassembly. Methods access
// fields via byte-offset casts from `this`.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct Station;

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
