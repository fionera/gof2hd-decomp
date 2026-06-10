#ifndef GOF2_NODE_H
#define GOF2_NODE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// @portable-fields

// Galaxy on Fire 2 -- Node class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class. Holds an Array<Node*>* at +0x00, a value at +0x04, and an int at +0x08.
// Field offsets recovered from the per-method target disassembly; accessed via byte-offset
// casts from `this`.
//
//   0x00  Array<Node*>* children
//   0x04  int           field_04 (cleared to 0 in ctor)
//   0x08  int           field_08 (= param_1 in ctor)


struct Node;

// Engine container Array<T> { uint32_t length; T* data; uint32_t cap; } -- 0xc bytes.


void *operator new(__SIZE_TYPE__ size);

extern "C" {
// Array<Node*>::Array(this) -- default ctor at 0x00075ee0.
void Node_ArrayNode_ctor(void *self);
// ArrayRelease<Node*>(Array*) -- 0x00075eec.
void Node_ArrayNode_release(void *arr);
}

// Field accessors via byte offset.
static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

struct Node {
    Node* field_0x4;                    // +0x4
    unsigned int field_0x30;            // +0x30
};
#endif
