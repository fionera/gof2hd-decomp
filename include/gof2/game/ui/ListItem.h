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

class ListItem {
public:
    Array<AbyssEngine::String *> *lines; // +0x0  text lines
    Agent*     agent;                   // +0x4
    BluePrint* bluePrint;               // +0x8
    Ship*      ship;                    // +0xc
    // item / selectable / slot / buttonKind are also read by Hud.cpp and HangarList.cpp via
    // their legacy field_0xNN names; an anonymous union keeps both spellings at one offset.
    union { Item* item; Item* field_0x10; }; // +0x10
    Mission*   mission;                 // +0x14
    PendingProduct* pendingProduct;     // +0x18
    EngString* name;                    // +0x1c owned String
    EngString* name2;                   // +0x20 owned String
    union { uint8_t selectable; uint8_t field_0x24; }; // +0x24 selectable flag
    union { int slot; int field_0x28; };// +0x28 slot index (-1 sentinel)
    int        field_0x2c;              // +0x2c (-1 sentinel)
    union { int buttonKind; int field_0x30; }; // +0x30 0=sell, 1=move-to-cargo
    int        imageIndex;              // +0x34 (-1 sentinel / image index)
    uint8_t    textButton;              // +0x38 textButton flag
    int        field_0x3c;              // +0x3c (-1 sentinel)
    int        field_0x40;              // +0x40 (-1 sentinel)
    uint8_t    text;                    // +0x44 text flag

    // ---- constructors (each demangles to ListItem::ListItem(...)) ----
    // Exactly one payload pointer is set per overload. The String overloads take the
    // engine String by opaque pointer (modeled as EngString internally). Three former
    // decompiler aliases collapsed into the overload they delegated to: ctor_Slot ->
    // ListItem(int), ctor_TextButton -> ListItem(String*,bool,int), ctor_ListItem ->
    // the copy constructor.
    ListItem(Agent *a);
    ListItem(Array<AbyssEngine::String *> *arr);
    ListItem(BluePrint *bp);
    ListItem(Item *it);
    ListItem(Mission *m);
    ListItem(PendingProduct *pp);
    ListItem(Ship *s);
    ListItem(const void *src);                      // ListItem(String*)
    ListItem(const void *p1, const void *p2);       // ListItem(String*, String*)
    ListItem(const void *src, bool b);              // ListItem(String*, bool)
    ListItem(const void *src, bool b, int v);       // ListItem(String*, bool, int)
    ListItem(const void *src, int v);               // ListItem(String*, int)
    ListItem(const ListItem &src);                  // copy constructor
    ListItem(int v);                                // also the "slot" entry
    ListItem(int a, int b);
    ListItem(int a, int b, const void *src);

    ~ListItem();

    // ---- methods (converted from free functions) ----
    bool checkCredits();
    int checkSlot();
    int checkSort();
    // Tail helper of checkSort(): forwards the requested item sort to the player ship,
    // which answers whether a matching equipment slot is available. Static because the
    // original fragment took the ship (not the ListItem) as its first argument.
    static int sortCmp(Ship *ship, int sort);
    int getIndex();
    int getNumLines();
    int getPrice();
    void * init();
    bool isBluePrint();
    bool isCargo();
    bool isImage();
    bool isItem();
    bool isMission();
    bool isMoveToCargoButton();
    bool isPendingProduct();
    uint8_t isSelectable();
    bool isSellButton();
    bool isShip();
    bool isSlot();
    bool isTab();
    uint8_t isText();
    uint8_t isTextButton();
};
#endif
