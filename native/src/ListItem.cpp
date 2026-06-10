#include "gof2/ListItem.h"
// gof2/PendingProduct.h is intentionally NOT included (it does not compile standalone); ListItem.h
// provides a minimal PendingProduct view with the one field this TU reads.

extern "C" void *ListItem_init(ListItem *self);

// Shared status singleton accessor (one of getShip()/getCredits() per call site).
struct Status {
    Ship *getShip();
    int   getCredits();
};
extern Status *status;

// ---- isMission_a67be.cpp ----
extern "C" bool ListItem_isMission(ListItem *self) {
    return self->field_0x14 != 0;
}

// ---- isItem_a678c.cpp ----
extern "C" bool ListItem_isItem(ListItem *self) {
    return self->field_0x10 != 0;
}

// ---- ListItem_a64fa.cpp ----
// ListItem::ListItem(BluePrint*)

extern "C" ListItem *ListItem_ctor_BluePrint(ListItem *self, BluePrint *bp) {
    ListItem_init(self);
    self->field_0x24 = 1;
    self->field_0x8 = bp;
    return self;
}

// ---- checkSort_a68b0.cpp ----
extern "C" int Item_getSort_li(Item *it);              // blx 0x71908
extern "C" int ListItem_sortCmp(Ship *ship, int sort); // tail b.w 0x1abe28

// ListItem::checkSort() — field 0x10 is reloaded (not cached) before getSort.
extern "C" int ListItem_checkSort(ListItem *self) {
    if (self->field_0x10 == 0)
        return 0;
    Ship *ship = status->getShip();
    int sort = Item_getSort_li(self->field_0x10);
    return ListItem_sortCmp(ship, sort);
}

// ---- ListItem_a66f4.cpp ----
// ListItem::ListItem(int, int) -> 0x28 = second arg, 0x2c = first arg, selectable=1

extern "C" ListItem *ListItem_ctor_int_int(ListItem *self, int a, int b) {
    ListItem_init(self);
    self->field_0x28 = b;
    self->field_0x2c = a;
    self->field_0x24 = 1;
    return self;
}

// ---- _ListItem_a6748.cpp ----
// ListItem::~ListItem() — for each of the two owned String* (at 0x1c, 0x20):
// if non-null, call its virtual function at vtable+4 (deleting dtor) then null it.
// Authored as a real C++ destructor so it demangles to ListItem::~ListItem().
ListItem::~ListItem() {
    void *p = this->field_0x1c;
    if (p) {
        (*(void (**)(void *))(*(char **)p + 4))(p);
        this->field_0x1c = 0;
    }
    p = this->field_0x20;
    if (p) {
        (*(void (**)(void *))(*(char **)p + 4))(p);
        this->field_0x20 = 0;
    }
}

// ---- isCargo_a6796.cpp ----
extern "C" bool ListItem_isCargo(ListItem *self) {
    return self->field_0x10 != 0;
}

// ---- getNumLines_a6822.cpp ----
// ldrb r1,[0x44]; cmp #0; itee eq; mov.eq 0; ldr.ne r0,[r0,#0]; ldr.ne r0,[r0,#0]
// if (!textFlag) return 0; else return **(int**)this  (first line count of the strings array)
extern "C" int ListItem_getNumLines(ListItem *self) {
    if (self->field_0x44 == 0)
        return 0;
    return **(int **)self;
}

// ---- isBluePrint_a6808.cpp ----
extern "C" bool ListItem_isBluePrint(ListItem *self) {
    return self->field_0x8 != 0;
}

// ---- isSelectable_a676e.cpp ----
// ldrb.w r0,[r0,#0x24]; bx lr
extern "C" uint8_t ListItem_isSelectable(ListItem *self) {
    return self->field_0x24;
}

// ---- ListItem_a6512.cpp ----
// ListItem::ListItem(Ship*)

extern "C" ListItem *ListItem_ctor_Ship(ListItem *self, Ship *s) {
    ListItem_init(self);
    self->field_0x24 = 1;
    self->field_0xc = s;
    return self;
}

// ---- checkSlot_a6870.cpp ----
extern "C" int Item_getType_li(Item *it);                  // blx 0x718fc
extern "C" int Ship_getFreeSlots_li(Ship *ship, int type); // blx 0x733e4

// ListItem::checkSlot()
extern "C" int ListItem_checkSlot(ListItem *self) {
    int r = 0;
    if (self->field_0x10 != 0) {
        Ship *ship = status->getShip();
        int type = Item_getType_li(self->field_0x10);
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
    self->field_0x1c = s;
    self->field_0x30 = v;
    self->field_0x24 = 0;
    return self;
}

// ---- ListItem_a6688.cpp ----
// ListItem::ListItem(int)  -> field 0x28

extern "C" ListItem *ListItem_ctor_int(ListItem *self, int v) {
    ListItem_init(self);
    self->field_0x24 = 1;
    self->field_0x28 = v;
    return self;
}

// ---- ListItem_a66a0.cpp ----
// ListItem::ListItem(String* p1, String* p2): 0x1c=new String(p1), 0x20=new String(p2),
// selectable=0.
extern "C" ListItem *ListItem_ctor_String_String(ListItem *self, const void *p1, const void *p2) {
    ListItem_init(self);
    EngString *s1 = new EngString(p1, false);
    self->field_0x1c = s1;
    EngString *s2 = new EngString(p2, false);
    self->field_0x20 = s2;
    self->field_0x24 = 0;
    return self;
}

// ---- isImage_a6774.cpp ----
// ldr r1,[r0,#0x34]; movs r0,#0; cmp.w r1,#0xffffffff; it gt; mov.gt r0,#1
extern "C" bool ListItem_isImage(ListItem *self) {
    return self->field_0x34 < 0x80000000u;
}

// ---- ListItem_a63e0.cpp ----
// ListItem::ListItem(int p1, int p2, String* p3): 0x34=p1, new String(p3), 0x28=p2,
// 0x1c=s, selectable=1.
extern "C" ListItem *ListItem_ctor_int_int_String(ListItem *self, int a, int b, const void *src) {
    ListItem_init(self);
    self->field_0x34 = a;
    EngString *s = new EngString(src, false);
    self->field_0x1c = s;
    self->field_0x28 = b;
    self->field_0x24 = 1;
    return self;
}

// ---- getIndex_a68fc.cpp ----
extern "C" int Ship_getIndex_li(Ship *s);    // tail b.w 0x1abe58
extern "C" int Item_getIndex_li(Item *it);    // tail b.w 0x1abe68
extern "C" int BluePrint_getIndex_li(BluePrint *bp);  // tail b.w 0x1abe78

// ListItem::getIndex() — default 999999 (literal pool constant) when nothing set;
// the PendingProduct path reads its index at +0x14, selected via ite eq.
extern "C" int ListItem_getIndex(ListItem *self) {
    Ship *sh = self->field_0xc;
    if (sh)
        return Ship_getIndex_li(sh);
    Item *it = self->field_0x10;
    if (it)
        return Item_getIndex_li(it);
    BluePrint *bp = self->field_0x8;
    if (bp)
        return BluePrint_getIndex_li(bp);
    PendingProduct *pp = (PendingProduct *)self->field_0x18;
    if (pp == 0)
        return 999999;
    return pp->field_0x14;
}

// ---- ListItem_a652a.cpp ----
// ListItem::ListItem(Item*)

extern "C" ListItem *ListItem_ctor_Item(ListItem *self, Item *it) {
    ListItem_init(self);
    self->field_0x24 = 1;
    self->field_0x10 = it;
    return self;
}

// ---- ListItem_a6730.cpp ----
// ListItem::ListItem(Array<String*>*)  -> field 0x00, selectable=0

extern "C" ListItem *ListItem_ctor_Array(ListItem *self, Array<AbyssEngine::String12 *> *arr) {
    ListItem_init(self);
    self->field_0x24 = 0;
    self->field_0x0 = arr;
    return self;
}

// ---- ListItem_a6718.cpp ----
// ListItem::ListItem(Mission*)

extern "C" ListItem *ListItem_ctor_Mission(ListItem *self, Mission *m) {
    ListItem_init(self);
    self->field_0x24 = 1;
    self->field_0x14 = m;
    return self;
}

// ---- isTextButton_a67d6.cpp ----
// ldrb.w r0,[r0,#0x38]; bx lr
extern "C" uint8_t ListItem_isTextButton(ListItem *self) {
    return self->field_0x38;
}

// ---- isText_a681c.cpp ----
// ldrb.w r0,[r0,#0x44]; bx lr
extern "C" uint8_t ListItem_isText(ListItem *self) {
    return self->field_0x44;
}

// ---- checkCredits_a6834.cpp ----
extern "C" int Ship_getPrice_li(Ship *s);          // blx 0x71b00
extern "C" int Item_getSinglePrice_li(Item *it);    // blx 0x71944

// ListItem::checkCredits()
extern "C" bool ListItem_checkCredits(ListItem *self) {
    int price;
    Ship *sh = self->field_0xc;
    if (sh == 0) {
        Item *it = self->field_0x10;
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
    self->field_0x1c = s;
    self->field_0x44 = b;
    self->field_0x24 = 0;
    return self;
}

// ---- getPrice_a68e4.cpp ----
// (Ship_getPrice_li / Item_getSinglePrice_li(Item*) already declared above.)

// ListItem::getPrice()
extern "C" int ListItem_getPrice(ListItem *self) {
    Ship *sh = self->field_0xc;
    if (sh)
        return Ship_getPrice_li(sh);
    if (self->field_0x10 == 0)
        return 0;
    return Item_getSinglePrice_li(self->field_0x10);
}

// ---- isSlot_a67c8.cpp ----
extern "C" bool ListItem_isSlot(ListItem *self) {
    return self->field_0x28 < 0x80000000u;
}

// ---- isMoveToCargoButton_a67f0.cpp ----
// if (!this[0x38]) false; else this[0x30]==1
extern "C" bool ListItem_isMoveToCargoButton(ListItem *self) {
    if (self->field_0x38 == 0)
        return false;
    return self->field_0x30 == 1;
}

// ---- isPendingProduct_a6812.cpp ----
extern "C" bool ListItem_isPendingProduct(ListItem *self) {
    return self->field_0x18 != 0;
}

// ---- isTab_a67a0.cpp ----
// if (!selectable && *(int*)(*(int*)(this+0x1c)+8) != 0) return this[0x38]==0; else false
extern "C" bool ListItem_isTab(ListItem *self) {
    if (self->field_0x24 == 0 &&
        *(int *)((char *)self->field_0x1c + 8) != 0)
        return self->field_0x38 == 0;
    return false;
}

// ---- isShip_a6782.cpp ----
// ldr r0,[r0,#0xc]; cmp; it ne; mov.ne r0,#1; bx lr
extern "C" bool ListItem_isShip(ListItem *self) {
    return self->field_0xc != 0;
}

// ---- isSellButton_a67dc.cpp ----
// if (!this[0x38]) false; else this[0x30]==0
extern "C" bool ListItem_isSellButton(ListItem *self) {
    if (self->field_0x38 == 0)
        return false;
    return self->field_0x30 == 0;
}

// ---- ListItem_a65bc.cpp ----
// ListItem::ListItem(String*) -> new String at 0x1c, selectable=0

extern "C" ListItem *ListItem_ctor_String(ListItem *self, const void *src) {
    ListItem_init(self);
    EngString *s = new EngString(src, false);
    self->field_0x1c = s;
    self->field_0x24 = 0;
    return self;
}

// ---- ListItem_a6542.cpp ----
// ListItem::ListItem(PendingProduct*)

extern "C" ListItem *ListItem_ctor_PendingProduct(ListItem *self, PendingProduct *pp) {
    ListItem_init(self);
    self->field_0x24 = 1;
    self->field_0x18 = pp;
    return self;
}

// ---- ListItem_a655a.cpp ----
// ListItem::ListItem(Agent*)

extern "C" ListItem *ListItem_ctor_Agent(ListItem *self, Agent *a) {
    ListItem_init(self);
    self->field_0x24 = 1;
    self->field_0x4 = a;
    return self;
}

// ---- ListItem_a6572.cpp ----
// ListItem::ListItem(String*, bool p2, int p3): 0x38=1, 0x1c=new String, 0x30=p3,
// selectable=p2.
extern "C" ListItem *ListItem_ctor_String_bool_int(ListItem *self, const void *src, bool b, int v) {
    ListItem_init(self);
    EngString *s = new EngString(src, false);
    self->field_0x38 = 1;
    self->field_0x1c = s;
    self->field_0x30 = v;
    self->field_0x24 = b;
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
    self->field_0x14 = 0;
    self->field_0x1c = 0;
    self->field_0x24 = 0;
    self->field_0x28 = -1;
    self->field_0x2c = -1;
    self->field_0x30 = -1;
    self->field_0x34 = -1;
    self->field_0x38 = 0;
    self->field_0x3c = -1;
    self->field_0x40 = -1;
    self->field_0x44 = 0;
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
    self->field_0x4 = src->field_0x4;
    self->field_0x14 = src->field_0x14;
    self->field_0x24 = src->field_0x24;

    void *p = src->field_0x1c;
    if (p)
        self->field_0x1c = new EngString(p, false);
    else
        self->field_0x1c = 0;
    p = src->field_0x20;
    if (p)
        self->field_0x20 = new EngString(p, false);
    else
        self->field_0x20 = 0;

    self->field_0x28 = src->field_0x28;
    self->field_0x38 = src->field_0x38;
    self->field_0x3c = src->field_0x3c;
    self->field_0x40 = src->field_0x40;
    self->field_0x18 = src->field_0x18;
    return self;
}
