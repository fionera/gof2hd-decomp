#include "gof2/game/ui/ListItem.h"
// gof2/PendingProduct.h is intentionally NOT included (it does not compile standalone); ListItem.h
// provides a minimal PendingProduct view with the one field this TU reads.

// Shared status singleton accessor (one of getShip()/getCredits() per call site).
struct Status {
    Ship *getShip();
    int   getCredits();
};
extern Status *status;

bool ListItem::isMission() {
    return this->mission != 0;
}

bool ListItem::isItem() {
    return this->item != 0;
}

// ListItem::ListItem(BluePrint*)

ListItem::ListItem(BluePrint *bp) {
    this->init();
    this->selectable = 1;
    this->bluePrint = bp;
}

extern "C" int Item_getSort_li(Item *it);              // blx 0x71908

// ListItem::checkSort() — field 0x10 is reloaded (not cached) before getSort.
int ListItem::checkSort() {
    if (this->item == 0)
        return 0;
    Ship *ship = status->getShip();
    int sort = Item_getSort_li(this->item);
    return ListItem::sortCmp(ship, sort);
}

// ListItem::sortCmp(Ship*, int) — the tail-called helper of checkSort(). The original
// fragment at 0x1abe28 is an ARM/Thumb interworking veneer that jumps straight into
// Ship::slotAvailable(int), so the entry pair simply asks the ship whether a slot
// matching this item's sort is free.
extern "C" int _ZN4Ship13slotAvailableEi(Ship *ship, int sort); // Ship::slotAvailable(int)
int ListItem::sortCmp(Ship *ship, int sort) {
    return _ZN4Ship13slotAvailableEi(ship, sort);
}

// ListItem::ListItem(int, int) -> 0x28 = second arg, 0x2c = first arg, selectable=1

ListItem::ListItem(int a, int b) {
    this->init();
    this->slot = b;
    this->field_0x2c = a;
    this->selectable = 1;
}

// ListItem::~ListItem() — for each of the two owned String* (at 0x1c, 0x20):
// if non-null, call its virtual function at vtable+4 (deleting dtor) then null it.
// Authored as a real C++ destructor so it demangles to ListItem::~ListItem().
ListItem::~ListItem() {
    void *p = this->name;
    if (p) {
        (*(void (**)(void *))(*(char **)p + 4))(p);
        this->name = 0;
    }
    p = this->name2;
    if (p) {
        (*(void (**)(void *))(*(char **)p + 4))(p);
        this->name2 = 0;
    }
}

bool ListItem::isCargo() {
    return this->item != 0;
}

// ldrb r1,[0x44]; cmp #0; itee eq; mov.eq 0; ldr.ne r0,[r0,#0]; ldr.ne r0,[r0,#0]
// if (!textFlag) return 0; else return **(int**)this  (first line count of the strings array)
int ListItem::getNumLines() {
    if (this->text == 0)
        return 0;
    return *(int *)this->lines;
}

bool ListItem::isBluePrint() {
    return this->bluePrint != 0;
}

// ldrb.w r0,[r0,#0x24]; bx lr
uint8_t ListItem::isSelectable() {
    return this->selectable;
}

// ListItem::ListItem(Ship*)

ListItem::ListItem(Ship *s) {
    this->init();
    this->selectable = 1;
    this->ship = s;
}

extern "C" int Item_getType_li(Item *it);                  // blx 0x718fc
extern "C" int Ship_getFreeSlots_li(Ship *ship, int type); // blx 0x733e4

// ListItem::checkSlot()
int ListItem::checkSlot() {
    int r = 0;
    if (this->item != 0) {
        Ship *ship = status->getShip();
        int type = Item_getType_li(this->item);
        if (Ship_getFreeSlots_li(ship, type) > 0)
            r = 1;
    }
    return r;
}

// ListItem::ListItem(String*, int) -> 0x30=int, 0x1c=new String, selectable=0
ListItem::ListItem(const void *src, int v) {
    this->init();
    EngString *s = new EngString(src, false);
    this->name = s;
    this->buttonKind = v;
    this->selectable = 0;
}

// ListItem::ListItem(int)  -> field 0x28

ListItem::ListItem(int v) {
    this->init();
    this->selectable = 1;
    this->slot = v;
}

// ListItem::ListItem(String* p1, String* p2): 0x1c=new String(p1), 0x20=new String(p2),
// selectable=0.
ListItem::ListItem(const void *p1, const void *p2) {
    this->init();
    EngString *s1 = new EngString(p1, false);
    this->name = s1;
    EngString *s2 = new EngString(p2, false);
    this->name2 = s2;
    this->selectable = 0;
}

// ldr r1,[r0,#0x34]; movs r0,#0; cmp.w r1,#0xffffffff; it gt; mov.gt r0,#1
bool ListItem::isImage() {
    return (unsigned)this->imageIndex < 0x80000000u;
}

// ListItem::ListItem(int p1, int p2, String* p3): 0x34=p1, new String(p3), 0x28=p2,
// 0x1c=s, selectable=1.
ListItem::ListItem(int a, int b, const void *src) {
    this->init();
    this->imageIndex = a;
    EngString *s = new EngString(src, false);
    this->name = s;
    this->slot = b;
    this->selectable = 1;
}

extern "C" int Ship_getIndex_li(Ship *s);    // tail b.w 0x1abe58
extern "C" int Item_getIndex_li(Item *it);    // tail b.w 0x1abe68
extern "C" int BluePrint_getIndex_li(BluePrint *bp);  // tail b.w 0x1abe78

// ListItem::getIndex() — default 999999 (literal pool constant) when nothing set;
// the PendingProduct path reads its index at +0x14, selected via ite eq.
int ListItem::getIndex() {
    Ship *sh = this->ship;
    if (sh)
        return Ship_getIndex_li(sh);
    Item *it = this->item;
    if (it)
        return Item_getIndex_li(it);
    BluePrint *bp = this->bluePrint;
    if (bp)
        return BluePrint_getIndex_li(bp);
    PendingProduct *pp = this->pendingProduct;
    if (pp == 0)
        return 999999;
    return pp->field_0x14;
}

// ListItem::ListItem(Item*)

ListItem::ListItem(Item *it) {
    this->init();
    this->selectable = 1;
    this->item = it;
}

// ListItem::ListItem(Array<String*>*)  -> field 0x00, selectable=0

ListItem::ListItem(Array<AbyssEngine::String *> *arr) {
    this->init();
    this->selectable = 0;
    this->lines = arr;
}

// ListItem::ListItem(Mission*)

ListItem::ListItem(Mission *m) {
    this->init();
    this->selectable = 1;
    this->mission = m;
}

// ldrb.w r0,[r0,#0x38]; bx lr
uint8_t ListItem::isTextButton() {
    return this->textButton;
}

// ldrb.w r0,[r0,#0x44]; bx lr
uint8_t ListItem::isText() {
    return this->text;
}

extern "C" int Ship_getPrice_li(Ship *s);          // blx 0x71b00
extern "C" int Item_getSinglePrice_li(Item *it);    // blx 0x71944

// ListItem::checkCredits()
bool ListItem::checkCredits() {
    int price;
    Ship *sh = this->ship;
    if (sh == 0) {
        Item *it = this->item;
        if (it == 0)
            return false;
        price = Item_getSinglePrice_li(it);
    } else {
        price = Ship_getPrice_li(sh);
    }
    int credits = status->getCredits();
    return price <= credits;
}

// ListItem::ListItem(String*, bool) -> 0x44=bool, 0x1c=new String, selectable=0
ListItem::ListItem(const void *src, bool b) {
    this->init();
    EngString *s = new EngString(src, false);
    this->name = s;
    this->text = b;
    this->selectable = 0;
}

// (Ship_getPrice_li / Item_getSinglePrice_li(Item*) already declared above.)

// ListItem::getPrice()
int ListItem::getPrice() {
    Ship *sh = this->ship;
    if (sh)
        return Ship_getPrice_li(sh);
    if (this->item == 0)
        return 0;
    return Item_getSinglePrice_li(this->item);
}

bool ListItem::isSlot() {
    return (unsigned)this->slot < 0x80000000u;
}

// if (!this[0x38]) false; else this[0x30]==1
bool ListItem::isMoveToCargoButton() {
    if (this->textButton == 0)
        return false;
    return this->buttonKind == 1;
}

bool ListItem::isPendingProduct() {
    return this->pendingProduct != 0;
}

// if (!selectable && *(int*)(*(int*)(this+0x1c)+8) != 0) return this[0x38]==0; else false
bool ListItem::isTab() {
    if (this->selectable == 0 &&
        *(int *)((char *)this->name + 8) != 0)
        return this->textButton == 0;
    return false;
}

// ldr r0,[r0,#0xc]; cmp; it ne; mov.ne r0,#1; bx lr
bool ListItem::isShip() {
    return this->ship != 0;
}

// if (!this[0x38]) false; else this[0x30]==0
bool ListItem::isSellButton() {
    if (this->textButton == 0)
        return false;
    return this->buttonKind == 0;
}

// ListItem::ListItem(String*) -> new String at 0x1c, selectable=0

ListItem::ListItem(const void *src) {
    this->init();
    EngString *s = new EngString(src, false);
    this->name = s;
    this->selectable = 0;
}

// ListItem::ListItem(PendingProduct*)

ListItem::ListItem(PendingProduct *pp) {
    this->init();
    this->selectable = 1;
    this->pendingProduct = pp;
}

// ListItem::ListItem(Agent*)

ListItem::ListItem(Agent *a) {
    this->init();
    this->selectable = 1;
    this->agent = a;
}

// ListItem::ListItem(String*, bool p2, int p3): 0x38=1, 0x1c=new String, 0x30=p3,
// selectable=p2.
ListItem::ListItem(const void *src, bool b, int v) {
    this->init();
    EngString *s = new EngString(src, false);
    this->textButton = 1;
    this->name = s;
    this->buttonKind = v;
    this->selectable = b;
}

// ListItem::init() — zero/sentinel-init all fields. Returns this+4.
// The this+4 16-byte block (fields 4,8,0xc,0x10) is the NEON store (vst1.32);
// the -1 sentinels (0x28..0x34, 0x3c..0x40) and the 0x14..0x20 zeros are strd
// pairs. NOTE: clang's SLP vectorizer additionally merges the 0x14..0x20 pair into
// a single NEON store here, which the target keeps as two strd — a residual
// 1-instruction-class difference we could not coax away at -Oz.
void * ListItem::init() {
    this->mission = 0;
    this->name = 0;
    this->selectable = 0;
    this->slot = -1;
    this->field_0x2c = -1;
    this->buttonKind = -1;
    this->imageIndex = -1;
    this->textButton = 0;
    this->field_0x3c = -1;
    this->field_0x40 = -1;
    this->text = 0;
    // 16-byte NEON store zeroing agent/bluePrint/ship/item (fields +0x4..+0x10).
    unsigned *p4 = (unsigned *)&this->agent;
    p4[0] = 0; p4[1] = 0; p4[2] = 0; p4[3] = 0;
    return (char *)&this->agent;  // address of member at +0x4
}

// ListItem::ListItem(ListItem* src) — copy constructor. Pointer/value fields are
// bulk-copied (16-byte NEON for 0x4..0x14 and 0x28..0x38); the two owned Strings
// at 0x1c/0x20 are deep-cloned when present.
ListItem::ListItem(const ListItem &src) {
    this->init();
    this->agent = src.agent;
    this->mission = src.mission;
    this->selectable = src.selectable;

    const void *p = src.name;
    if (p)
        this->name = new EngString(p, false);
    else
        this->name = 0;
    p = src.name2;
    if (p)
        this->name2 = new EngString(p, false);
    else
        this->name2 = 0;

    this->slot = src.slot;
    this->textButton = src.textButton;
    this->field_0x3c = src.field_0x3c;
    this->field_0x40 = src.field_0x40;
    this->pendingProduct = src.pendingProduct;
}

// The decompiler's ctor_Slot (b6688), ctor_TextButton (b6572) and ctor_ListItem
// (b645a) were per-call-site aliases with bodies identical to ListItem(int),
// ListItem(String*,bool,int) and the copy constructor respectively; they collapse
// into those overloads. Their call sites now construct the canonical overload.
