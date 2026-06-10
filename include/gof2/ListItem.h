#ifndef GOF2_LISTITEM_H
#define GOF2_LISTITEM_H
#include "gof2/common.h"
// Galaxy on Fire 2 -- ListItem (Android libgof2hdaa.so, armv7 Thumb).
// A polymorphic entry in the various menu lists. Exactly one of its payload pointers is set by
// the matching constructor (Array of text lines at 0x0, Agent 0x4, BluePrint 0x8, Ship 0xc,
// Item 0x10, Mission 0x14, PendingProduct 0x18), plus up to two owned name Strings at 0x1c/0x20
// and a set of flags/ints (selectable 0x24, ints 0x28..0x40, textButton flag 0x38, text flag 0x44).
//
// Top-level class (no AbyssEngine:: namespace). Most methods are authored as extern "C" wrappers;
// the destructor is a real C++ member so it demangles to ListItem::~ListItem().

// Opaque referenced engine types (full layout not modeled).
struct Ship;
struct Item;
struct Mission;
struct Agent;
struct BluePrint;

// gof2/PendingProduct.h does not compile standalone, and ListItem only reads its index field, so
// we complete the global forward declaration here with a minimal, layout-faithful view.
struct PendingProduct {
    char pad_0x0[0x14];
    int  field_0x14;                    // +0x14 product/blueprint index
};

// Engine String, modeled with just the copy-ctor ListItem needs (heap-allocated name Strings).
struct EngString {
    uint32_t a, b, c;
    EngString(const void *src, bool copy);
};

struct ListItem {
    void*    field_0x0;                 // +0x0  Array<String*>* (text lines)
    void*    field_0x4;                 // +0x4  Agent*
    BluePrint* field_0x8;               // +0x8
    Ship*    field_0xc;                 // +0xc
    Item*    field_0x10;                // +0x10
    void*    field_0x14;                // +0x14 Mission*
    void*    field_0x18;                // +0x18 PendingProduct*
    void*    field_0x1c;                // +0x1c owned String*
    void*    field_0x20;                // +0x20 owned String*
    uint8_t  field_0x24;                // +0x24 selectable flag
    int      field_0x28;                // +0x28
    int      field_0x2c;                // +0x2c
    int      field_0x30;                // +0x30
    int      field_0x34;                // +0x34 (-1 sentinel / image index)
    uint8_t  field_0x38;                // +0x38 textButton flag
    int      field_0x3c;                // +0x3c
    int      field_0x40;                // +0x40
    uint8_t  field_0x44;                // +0x44 text flag

    ~ListItem();
};
#endif
