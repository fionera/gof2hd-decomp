#ifndef GOF2_LISTITEM_H
#define GOF2_LISTITEM_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — ListItem class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: namespace; only some field/arg types live in
// AbyssEngine). Field offsets recovered from per-method target disassembly. We do
// NOT model a full layout — methods access fields via byte-offset casts from `this`.
//
// Known field offsets (all pointers unless noted):
//   0x00 Array<String*>* strings   0x04 Agent*    0x08 BluePrint*  0x0c Ship*
//   0x10 Item*    0x14 Mission*     0x18 PendingProduct*
//   0x1c String*  0x20 String*      (both owned, deleted in dtor via vtable+4)
//   0x24 uint8  selectable flag     0x28 int       0x2c int        0x30 int
//   0x34 uint  (-1 sentinel)        0x38 uint8 textButton flag
//   0x3c int      0x40 int          0x44 uint8 text flag (+0x45 also touched as halfword)


// Opaque referenced engine types.
struct Ship;
struct Item;
struct Mission;
struct Agent;
struct BluePrint;
struct PendingProduct;

// AbyssEngine::String — 12-byte object (text*, size, ...). Heap-allocated members
// at 0x1c/0x20. Modeled opaquely; constructed via the engine helpers below.


// 16-byte aggregate of four 32-bit words — copied as a unit so clang emits a
// vld1.32/vst1.32 NEON move (matching the target's bulk field copies).
struct Blk16 { int w[4]; };

// Engine String, modeled with the copy-ctor we need so `new EngString(src,false)`
// emits operator new(0xc) + String(const String&, bool) exactly like the target.
struct EngString {
    uint32_t a, b, c;
    EngString(const void *src, bool copy);
};
void *operator new(__SIZE_TYPE__);



// ListItem owns two heap String* (0x1c, 0x20), each with a virtual deleting dtor.
// We declare the destructor so it demangles to ListItem::~ListItem(); all other
// methods are authored as extern "C" wrappers.


// Field accessor via byte offset.

struct ListItem {
    Agent* field_0x4;                   // +0x4
    BluePrint* field_0x8;               // +0x8
    Ship* field_0xc;                    // +0xc
    Item* field_0x10;                   // +0x10
    int field_0x14;                     // +0x14
    int field_0x18;                     // +0x18
    void* field_0x1c;                   // +0x1c
    void* field_0x20;                   // +0x20
    uint8_t field_0x24;                 // +0x24
    int field_0x28;                     // +0x28
    int field_0x2c;                     // +0x2c
    int field_0x30;                     // +0x30
    int field_0x34;                     // +0x34
    uint8_t field_0x38;                 // +0x38
    int field_0x3c;                     // +0x3c
    int field_0x40;                     // +0x40
    uint8_t field_0x44;                 // +0x44
};
#endif
