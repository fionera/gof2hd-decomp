#ifndef WORK_CLASSES_LISTITEM_SRC_CLASS_H
#define WORK_CLASSES_LISTITEM_SRC_CLASS_H

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

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

// Opaque referenced engine types.
struct Ship;
struct Item;
struct Mission;
struct Agent;
struct BluePrint;
struct PendingProduct;

// AbyssEngine::String — 12-byte object (text*, size, ...). Heap-allocated members
// at 0x1c/0x20. Modeled opaquely; constructed via the engine helpers below.
struct String12 { uint32_t a, b, c; };

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

template <class T>
struct Array {
    uint32_t length;   // +0x00
    T *data;           // +0x04
    uint32_t cap;      // +0x08
};

// ListItem owns two heap String* (0x1c, 0x20), each with a virtual deleting dtor.
// We declare the destructor so it demangles to ListItem::~ListItem(); all other
// methods are authored as extern "C" wrappers.
struct ListItem {
    ~ListItem();
};

// Field accessor via byte offset.
template <class T>
static inline T &F(ListItem *self, unsigned off) { return *(T *)((char *)self + off); }

#endif
