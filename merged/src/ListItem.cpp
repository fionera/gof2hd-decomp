#include "ListItem.h"


extern "C" void *ListItem_init(ListItem *self);

// ---- isMission_a67be.cpp ----
extern "C" bool ListItem_isMission(ListItem *self) {
    return F<int>(self, 0x14) != 0;
}

// ---- isItem_a678c.cpp ----
extern "C" bool ListItem_isItem(ListItem *self) {
    return F<int>(self, 0x10) != 0;
}

// ---- ListItem_a64fa.cpp ----
// ListItem::ListItem(BluePrint*)

extern "C" ListItem *ListItem_ctor_BluePrint(ListItem *self, BluePrint *bp) {
    ListItem_init(self);
    F<uint8_t>(self, 0x24) = 1;
    F<BluePrint *>(self, 0x8) = bp;
    return self;
}

// ---- checkSort_a68b0.cpp ----
struct Status { Ship *getShip(); };
extern Status *status;
extern "C" int Item_getSort_li(Item *it);              // blx 0x71908
extern "C" int ListItem_sortCmp(Ship *ship, int sort); // tail b.w 0x1abe28

// ListItem::checkSort() — field 0x10 is reloaded (not cached) before getSort.
extern "C" int ListItem_checkSort(ListItem *self) {
    if (F<Item *>(self, 0x10) == 0)
        return 0;
    Ship *ship = status->getShip();
    int sort = Item_getSort_li(F<Item *>(self, 0x10));
    return ListItem_sortCmp(ship, sort);
}

// ---- ListItem_a66f4.cpp ----
// ListItem::ListItem(int, int) -> 0x28 = second arg, 0x2c = first arg, selectable=1

extern "C" ListItem *ListItem_ctor_int_int(ListItem *self, int a, int b) {
    ListItem_init(self);
    F<int>(self, 0x28) = b;
    F<int>(self, 0x2c) = a;
    F<uint8_t>(self, 0x24) = 1;
    return self;
}

// ---- _ListItem_a6748.cpp ----
// ListItem::~ListItem() — for each of the two owned String* (at 0x1c, 0x20):
// if non-null, call its virtual function at vtable+4 (deleting dtor) then null it.
// Authored as a real C++ destructor so it demangles to ListItem::~ListItem().
ListItem::~ListItem() {
    void **p = F<void **>(this, 0x1c);
    if (p) {
        (*(void (**)(void *))(*(char **)p + 4))(p);
        F<void *>(this, 0x1c) = 0;
    }
    p = F<void **>(this, 0x20);
    if (p) {
        (*(void (**)(void *))(*(char **)p + 4))(p);
        F<void *>(this, 0x20) = 0;
    }
}

// ---- isCargo_a6796.cpp ----
extern "C" bool ListItem_isCargo(ListItem *self) {
    return F<int>(self, 0x10) != 0;
}

// ---- getNumLines_a6822.cpp ----
// ldrb r1,[0x44]; cmp #0; itee eq; mov.eq 0; ldr.ne r0,[r0,#0]; ldr.ne r0,[r0,#0]
// if (!textFlag) return 0; else return **(int**)this  (first line count of the strings array)
extern "C" int ListItem_getNumLines(ListItem *self) {
    if (F<uint8_t>(self, 0x44) == 0)
        return 0;
    return **(int **)self;
}

// ---- isBluePrint_a6808.cpp ----
extern "C" bool ListItem_isBluePrint(ListItem *self) {
    return F<int>(self, 0x8) != 0;
}

// ---- isSelectable_a676e.cpp ----
// ldrb.w r0,[r0,#0x24]; bx lr
extern "C" uint8_t ListItem_isSelectable(ListItem *self) {
    return F<uint8_t>(self, 0x24);
}

// ---- ListItem_a6512.cpp ----
// ListItem::ListItem(Ship*)

extern "C" ListItem *ListItem_ctor_Ship(ListItem *self, Ship *s) {
    ListItem_init(self);
    F<uint8_t>(self, 0x24) = 1;
    F<Ship *>(self, 0xc) = s;
    return self;
}

// ---- checkSlot_a6870.cpp ----
struct Status { Ship *getShip(); };
extern Status *status;
extern "C" int Item_getType_li(Item *it);                  // blx 0x718fc
extern "C" int Ship_getFreeSlots_li(Ship *ship, int type); // blx 0x733e4

// ListItem::checkSlot()
extern "C" int ListItem_checkSlot(ListItem *self) {
    int r = 0;
    if (F<Item *>(self, 0x10) != 0) {
        Ship *ship = status->getShip();
        int type = Item_getType_li(F<Item *>(self, 0x10));
        if (Ship_getFreeSlots_li(ship, type) > 0)
            r = 1;
    }
    return r;
}

// ---- ListItem_a65fc.cpp ----
// ListItem::ListItem(String*, int) -> 0x30=int, 0x1c=new String, selectable=0
extern "C" ListItem *ListItem_ctor_String_int(ListItem *self, const void *src, int v) {
    ListItem_init(self);
    EngString *s = new EngString(src, false);
    F<void *>(self, 0x1c) = s;
    F<int>(self, 0x30) = v;
    F<uint8_t>(self, 0x24) = 0;
    return self;
}

// ---- ListItem_a6688.cpp ----
// ListItem::ListItem(int)  -> field 0x28

extern "C" ListItem *ListItem_ctor_int(ListItem *self, int v) {
    ListItem_init(self);
    F<uint8_t>(self, 0x24) = 1;
    F<int>(self, 0x28) = v;
    return self;
}

// ---- ListItem_a66a0.cpp ----
// ListItem::ListItem(String* p1, String* p2): 0x1c=new String(p1), 0x20=new String(p2),
// selectable=0.
extern "C" ListItem *ListItem_ctor_String_String(ListItem *self, const void *p1, const void *p2) {
    ListItem_init(self);
    EngString *s1 = new EngString(p1, false);
    F<void *>(self, 0x1c) = s1;
    EngString *s2 = new EngString(p2, false);
    F<void *>(self, 0x20) = s2;
    F<uint8_t>(self, 0x24) = 0;
    return self;
}

// ---- isImage_a6774.cpp ----
// ldr r1,[r0,#0x34]; movs r0,#0; cmp.w r1,#0xffffffff; it gt; mov.gt r0,#1
extern "C" bool ListItem_isImage(ListItem *self) {
    return F<uint32_t>(self, 0x34) < 0x80000000u;
}

// ---- ListItem_a63e0.cpp ----
// ListItem::ListItem(int p1, int p2, String* p3): 0x34=p1, new String(p3), 0x28=p2,
// 0x1c=s, selectable=1.
extern "C" ListItem *ListItem_ctor_int_int_String(ListItem *self, int a, int b, const void *src) {
    ListItem_init(self);
    F<int>(self, 0x34) = a;
    EngString *s = new EngString(src, false);
    F<void *>(self, 0x1c) = s;
    F<int>(self, 0x28) = b;
    F<uint8_t>(self, 0x24) = 1;
    return self;
}

// ---- getIndex_a68fc.cpp ----
extern "C" int Ship_getIndex_li(Ship *s);    // tail b.w 0x1abe58
extern "C" int Item_getIndex_li(Item *it);    // tail b.w 0x1abe68
extern "C" int BluePrint_getIndex_li(BluePrint *bp);  // tail b.w 0x1abe78

// ListItem::getIndex() — default 999999 (literal pool constant) when nothing set;
// the PendingProduct path reads its index at +0x14, selected via ite eq.
extern "C" int ListItem_getIndex(ListItem *self) {
    Ship *sh = F<Ship *>(self, 0xc);
    if (sh)
        return Ship_getIndex_li(sh);
    Item *it = F<Item *>(self, 0x10);
    if (it)
        return Item_getIndex_li(it);
    BluePrint *bp = F<BluePrint *>(self, 0x8);
    if (bp)
        return BluePrint_getIndex_li(bp);
    void *pp = F<void *>(self, 0x18);
    if (pp == 0)
        return 999999;
    return *(int *)((char *)pp + 0x14);
}

// ---- ListItem_a652a.cpp ----
// ListItem::ListItem(Item*)

extern "C" ListItem *ListItem_ctor_Item(ListItem *self, Item *it) {
    ListItem_init(self);
    F<uint8_t>(self, 0x24) = 1;
    F<Item *>(self, 0x10) = it;
    return self;
}

// ---- ListItem_a6730.cpp ----
// ListItem::ListItem(Array<String*>*)  -> field 0x00, selectable=0

extern "C" ListItem *ListItem_ctor_Array(ListItem *self, Array<String12 *> *arr) {
    ListItem_init(self);
    F<uint8_t>(self, 0x24) = 0;
    F<Array<String12 *> *>(self, 0x0) = arr;
    return self;
}

// ---- ListItem_a6718.cpp ----
// ListItem::ListItem(Mission*)

extern "C" ListItem *ListItem_ctor_Mission(ListItem *self, Mission *m) {
    ListItem_init(self);
    F<uint8_t>(self, 0x24) = 1;
    F<Mission *>(self, 0x14) = m;
    return self;
}

// ---- isTextButton_a67d6.cpp ----
// ldrb.w r0,[r0,#0x38]; bx lr
extern "C" uint8_t ListItem_isTextButton(ListItem *self) {
    return F<uint8_t>(self, 0x38);
}

// ---- isText_a681c.cpp ----
// ldrb.w r0,[r0,#0x44]; bx lr
extern "C" uint8_t ListItem_isText(ListItem *self) {
    return F<uint8_t>(self, 0x44);
}

// ---- checkCredits_a6834.cpp ----
struct Status { int getCredits(); };
extern Status *status;
extern "C" int Ship_getPrice_li(Ship *s);          // blx 0x71b00
extern "C" int Item_getSinglePrice_li(Item *it);    // blx 0x71944

// ListItem::checkCredits()
extern "C" bool ListItem_checkCredits(ListItem *self) {
    int price;
    Ship *sh = F<Ship *>(self, 0xc);
    if (sh == 0) {
        Item *it = F<Item *>(self, 0x10);
        if (it == 0)
            return false;
        price = Item_getSinglePrice_li(it);
    } else {
        price = Ship_getPrice_li(sh);
    }
    int credits = status->getCredits();
    return price <= credits;
}

// ---- ListItem_a6642.cpp ----
// ListItem::ListItem(String*, bool) -> 0x44=bool, 0x1c=new String, selectable=0
extern "C" ListItem *ListItem_ctor_String_bool(ListItem *self, const void *src, bool b) {
    ListItem_init(self);
    EngString *s = new EngString(src, false);
    F<void *>(self, 0x1c) = s;
    F<uint8_t>(self, 0x44) = b;
    F<uint8_t>(self, 0x24) = 0;
    return self;
}

// ---- getPrice_a68e4.cpp ----
extern "C" int Ship_getPrice_li(Ship *s);     // 0x1abe38 tail veneer
extern "C" int Item_getSinglePrice_li();       // 0x1abe48 tail veneer

// ListItem::getPrice()
extern "C" int ListItem_getPrice(ListItem *self) {
    Ship *sh = F<Ship *>(self, 0xc);
    if (sh)
        return Ship_getPrice_li(sh);
    if (F<int>(self, 0x10) == 0)
        return 0;
    return Item_getSinglePrice_li();
}

// ---- isSlot_a67c8.cpp ----
extern "C" bool ListItem_isSlot(ListItem *self) {
    return F<uint32_t>(self, 0x28) < 0x80000000u;
}

// ---- isMoveToCargoButton_a67f0.cpp ----
// if (!this[0x38]) false; else this[0x30]==1
extern "C" bool ListItem_isMoveToCargoButton(ListItem *self) {
    if (F<uint8_t>(self, 0x38) == 0)
        return false;
    return F<int>(self, 0x30) == 1;
}

// ---- isPendingProduct_a6812.cpp ----
extern "C" bool ListItem_isPendingProduct(ListItem *self) {
    return F<int>(self, 0x18) != 0;
}

// ---- isTab_a67a0.cpp ----
// if (!selectable && *(int*)(*(int*)(this+0x1c)+8) != 0) return this[0x38]==0; else false
extern "C" bool ListItem_isTab(ListItem *self) {
    if (F<uint8_t>(self, 0x24) == 0 &&
        *(int *)(F<int>(self, 0x1c) + 8) != 0)
        return F<uint8_t>(self, 0x38) == 0;
    return false;
}

// ---- isShip_a6782.cpp ----
// ldr r0,[r0,#0xc]; cmp; it ne; mov.ne r0,#1; bx lr
extern "C" bool ListItem_isShip(ListItem *self) {
    return F<int>(self, 0xc) != 0;
}

// ---- isSellButton_a67dc.cpp ----
// if (!this[0x38]) false; else this[0x30]==0
extern "C" bool ListItem_isSellButton(ListItem *self) {
    if (F<uint8_t>(self, 0x38) == 0)
        return false;
    return F<int>(self, 0x30) == 0;
}

// ---- ListItem_a65bc.cpp ----
// ListItem::ListItem(String*) -> new String at 0x1c, selectable=0

extern "C" ListItem *ListItem_ctor_String(ListItem *self, const void *src) {
    ListItem_init(self);
    EngString *s = new EngString(src, false);
    F<void *>(self, 0x1c) = s;
    F<uint8_t>(self, 0x24) = 0;
    return self;
}

// ---- ListItem_a6542.cpp ----
// ListItem::ListItem(PendingProduct*)

extern "C" ListItem *ListItem_ctor_PendingProduct(ListItem *self, PendingProduct *pp) {
    ListItem_init(self);
    F<uint8_t>(self, 0x24) = 1;
    F<PendingProduct *>(self, 0x18) = pp;
    return self;
}

// ---- ListItem_a655a.cpp ----
// ListItem::ListItem(Agent*)

extern "C" ListItem *ListItem_ctor_Agent(ListItem *self, Agent *a) {
    ListItem_init(self);
    F<uint8_t>(self, 0x24) = 1;
    F<Agent *>(self, 0x4) = a;
    return self;
}

// ---- ListItem_a6572.cpp ----
// ListItem::ListItem(String*, bool p2, int p3): 0x38=1, 0x1c=new String, 0x30=p3,
// selectable=p2.
extern "C" ListItem *ListItem_ctor_String_bool_int(ListItem *self, const void *src, bool b, int v) {
    ListItem_init(self);
    EngString *s = new EngString(src, false);
    F<uint8_t>(self, 0x38) = 1;
    F<void *>(self, 0x1c) = s;
    F<int>(self, 0x30) = v;
    F<uint8_t>(self, 0x24) = b;
    return self;
}

// ---- init_a6428.cpp ----
// ListItem::init() — zero/sentinel-init all fields. Returns this+4.
// The this+4 16-byte block (fields 4,8,0xc,0x10) is the NEON store (vst1.32);
// the -1 sentinels (0x28..0x34, 0x3c..0x40) and the 0x14..0x20 zeros are strd
// pairs. NOTE: clang's SLP vectorizer additionally merges the 0x14..0x20 pair into
// a single NEON store here, which the target keeps as two strd — a residual
// 1-instruction-class difference we could not coax away at -Oz.
extern "C" void *ListItem_init(ListItem *self) {
    F<long long>(self, 0x14) = 0;
    F<long long>(self, 0x1c) = 0;
    F<uint8_t>(self, 0x24) = 0;
    F<int>(self, 0x28) = -1;
    F<int>(self, 0x2c) = -1;
    F<int>(self, 0x30) = -1;
    F<int>(self, 0x34) = -1;
    F<uint8_t>(self, 0x38) = 0;
    F<int>(self, 0x3c) = -1;
    F<int>(self, 0x40) = -1;
    F<uint16_t>(self, 0x44) = 0;
    unsigned *p4 = (unsigned *)((char *)self + 4);
    p4[0] = 0; p4[1] = 0; p4[2] = 0; p4[3] = 0;
    return (char *)self + 4;
}

// ---- ListItem_a645a.cpp ----
// ListItem::ListItem(ListItem* src) — copy constructor. Pointer/value fields are
// bulk-copied (16-byte NEON for 0x4..0x14 and 0x28..0x38); the two owned Strings
// at 0x1c/0x20 are deep-cloned when present.
extern "C" ListItem *ListItem_ctor_copy(ListItem *self, ListItem *src) {
    ListItem_init(self);
    F<Blk16>(self, 0x4) = F<Blk16>(src, 0x4);
    F<int>(self, 0x14) = F<int>(src, 0x14);
    F<uint8_t>(self, 0x24) = F<uint8_t>(src, 0x24);

    void *p = F<void *>(src, 0x1c);
    if (p)
        F<void *>(self, 0x1c) = new EngString(p, false);
    else
        F<void *>(self, 0x1c) = 0;
    p = F<void *>(src, 0x20);
    if (p)
        F<void *>(self, 0x20) = new EngString(p, false);
    else
        F<void *>(self, 0x20) = 0;

    F<Blk16>(self, 0x28) = F<Blk16>(src, 0x28);
    F<uint8_t>(self, 0x38) = F<uint8_t>(src, 0x38);
    F<int>(self, 0x3c) = F<int>(src, 0x3c);
    F<int>(self, 0x40) = F<int>(src, 0x40);
    F<int>(self, 0x18) = F<int>(src, 0x18);
    return self;
}
