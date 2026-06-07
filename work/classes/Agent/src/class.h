#ifndef WORK_CLASSES_AGENT_SRC_CLASS_H
#define WORK_CLASSES_AGENT_SRC_CLASS_H

// Galaxy on Fire 2 — Agent class (NPC/trader). Top-level, no namespace.
// Field offsets recovered from per-method target disassembly. We do NOT model a
// full layout: methods access fields via byte-offset casts from `this`.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct Agent;

// AbyssEngine::String passed by value is a 12-byte trivially-copied aggregate
// (text*, size, ...). Model it opaquely so it is passed on the stack/regs the
// same way as the target ABI (struct >8 bytes by value).
struct String12 { uint32_t a, b, c; };

template <class T>
struct Array {
    uint32_t length;   // +0x00
    T *data;           // +0x04
    uint32_t cap;      // +0x08
};

// Field accessors via byte offsets.
template <class T>
static inline T &F(Agent *self, unsigned off) { return *(T *)((char *)self + off); }

#endif
