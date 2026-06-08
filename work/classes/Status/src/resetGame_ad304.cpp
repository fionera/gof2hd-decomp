#include "class.h"

// Local offset-cast helpers (the Status header models fields by name, but resetGame
// touches many raw offsets so byte-offset access is clearer here).
static inline int&   I(void *p, int off) { return *(int *)((char *)p + off); }
static inline char&  C(void *p, int off) { return *(char *)((char *)p + off); }
static inline void*& P(void *p, int off) { return *(void **)((char *)p + off); }

struct Standing;
struct Galaxy;
struct PendingProduct;
struct Achievements;

// PC-relative singleton / global slots referenced by resetGame.
__attribute__((visibility("hidden"))) extern int   *g_rg_settings;       // [DAT_000bd694] -> settings record
__attribute__((visibility("hidden"))) extern Galaxy **g_rg_galaxy;       // [DAT_000bd69c] -> Galaxy*
__attribute__((visibility("hidden"))) extern int  **g_rg_itemTable;      // [DAT_000bd980] -> Array<Item*>*
__attribute__((visibility("hidden"))) extern Achievements **g_rg_ach;    // [DAT_000bd6a0]
__attribute__((visibility("hidden"))) extern int  **g_rg_statusSlotA;    // [DAT_000bd994]
__attribute__((visibility("hidden"))) extern int  **g_rg_statusSlotB;    // [DAT_000bd988]
__attribute__((visibility("hidden"))) extern int  **g_rg_statusSlotC;    // [DAT_000bd984]
__attribute__((visibility("hidden"))) extern int  **g_rg_zeroSlotA;      // [DAT_000bd6a4]
__attribute__((visibility("hidden"))) extern char **g_rg_zeroSlotB;      // [DAT_000bd6a8]
__attribute__((visibility("hidden"))) extern int  **g_rg_zeroSlotC;      // [DAT_000bd6ac]
__attribute__((visibility("hidden"))) extern int (*g_rg_makeItemB)(int); // [DAT_000bd98c]
__attribute__((visibility("hidden"))) extern void (*g_rg_addCargo)(int, int, int); // [DAT_000bd990]

extern "C" {
void  *Status_opnew(unsigned int size);
void   operator_delete(void *p);
void   operator_delete_arr(void *p);
void  *Station_dtor(Station *s);
void   Station_ctor(Station *s);
void  *ArrayString_dtor(void *a);
void   ArrayReleaseClasses_String(void *a);
void  *Array_int_dtor(void *a);
void   Array_int_ctor(void *a);
void   ArraySetLength_int(int len, void *a);
void  *Array_bool_dtor(void *a);
void   Array_bool_ctor(void *a);
void   ArraySetLength_bool(int len, void *a);
void  *ArrayBP_dtor(void *a);
void   ArrayBP_ctor(void *a);
void   ArraySetLength_BP(int len, void *a);
void   ArrayReleaseClasses_BP(void *a);
void  *ArrayPP_dtor(void *a);
void   ArrayReleaseClasses_PP(void *a);
void   Globals_resetHints();
void   Galaxy_reset(Galaxy *g);
int   *Galaxy_getSystems(Galaxy *g);
void  *Galaxy_getStation(int gptr);
int    SolarSystem_isVisible(void *ss);
int    Item_getIngredients();
void   BluePrint_ctor(void *bp, unsigned int index);
void   Standing_ctor(Standing *s);
void  *Standing_dtor(Standing *s);
void   Achievements_init(Achievements *a, int v);
void   Mission_ctor(void *m, int a, int b, int c);
int    Ship_makeShip(int shipDesc);
void   Ship_priceDecline(Ship *s);
void   Ship_setCargo(Ship *s, void *cargo);
int    Ship_getMaxHP(Ship *s);
int    Ship_getMaxShieldHP();
int    Ship_getMaxArmorHP(Ship *s);
int    Item_makeItem(int itemDesc);
void   Status_rg_loadAgents(Status *self);
void   Status_rg_loadWanted(Status *self);
void   Status_rg_setCampaignMission(Status *self, void *m);
void   Status_rg_setShip(Status *self, int shipPtr);
void   Status_rg_setStation(Status *self, void *st);
}

// Status::resetGame()
//   Wipes the persistent player state back to a fresh new-game baseline: zeroes counters,
//   recreates the bonus/visibility arrays, blueprint list, agents/wanted, standing, the
//   starting mission, ship and station.
void Status::resetGame()
{
    char *self = (char *)this;
    int *settings = g_rg_settings;

    I(self, 0x8c)  = 0;
    char *srec = (char *)*settings;
    I(self, 0x1e4) = 0;
    I(self, 0x100) = 0;
    I(self, 0x104) = 0;
    I(self, 0x1b0) = 0;
    I(self, 0x1b4) = 0;
    I(self, 0x1b8) = 0;
    I(self, 0x1bc) = 0;
    I(self, 0x1c0) = 0;
    I(self, 0x1c4) = 0;
    I(self, 0x1c8) = 1;
    I(self, 0x1cc) = 0;
    I(self, 0x1d0) = 0;
    I(self, 0x10c) = 0;
    C(self, 0x110) = 0;
    I(self, 0x118) = 0;
    I(self, 0x11c) = 0;
    I(self, 0x124) = 0;
    I(self, 0x12c) = 0;
    // 0x1d4..0x1e0 set from a zero vector
    I(self, 0x1d4) = 0;
    I(self, 0x1d8) = 0;
    I(self, 0x1dc) = 0;
    I(self, 0x1e0) = 0;
    I(self, 0x13c) = 0;
    I(self, 0x134) = 0;
    I(self, 0x144) = 0;
    C(self, 0x120) = 0;
    C(self, 0x128) = 0;
    C(self, 0x130) = 0;
    C(self, 0x138) = 0;
    C(self, 0x140) = 0;
    C(self, 0x148) = 0;

    char hardcore = C(srec, 0x36);
    C(self, 0x111) = 0;
    I(self, 0x114) = hardcore != 0 ? 3 : 0;

    // recreate the per-game scratch station at 0x14c.
    if (P(self, 0x14c) != 0) {
        operator_delete(Station_dtor((Station *)P(self, 0x14c)));
        I(self, 0x14c) = 0;
    }
    Station *scratch = (Station *)Status_opnew(0x34);
    Station_ctor(scratch);
    P(self, 0x14c) = scratch;

    if (P(self, 0x28) != 0) {
        operator_delete_arr(P(self, 0x28));
        I(self, 0x28) = 0;
    }
    if (P(self, 0x24) != 0) {
        ArrayReleaseClasses_String(P(self, 0x24));
        if (P(self, 0x24) != 0)
            operator_delete(ArrayString_dtor(P(self, 0x24)));
    }

    // clear several bool arrays.
    int *ba = (int *)P(self, 0x94);
    I(self, 0x30) = 0;
    I(self, 0x24) = 0;
    for (int j = 0; *ba != j; j = j + 1)
        ((char *)ba[1])[j] = 0;

    ba = (int *)P(self, 0x98);
    for (int j = 0; *ba != j; j = j + 1)
        ((char *)ba[1])[j] = 0;

    ba = (int *)P(self, 0xac);
    I(self, 0x9c) = 0;
    I(self, 0xa0) = 0;
    I(self, 0xa4) = 0;
    I(self, 0xa8) = 0;
    for (int j = 0; *ba != j; j = j + 1)
        ((char *)ba[1])[j] = 0;

    ba = (int *)P(self, 0xb4);
    I(self, 0xb0) = 0;
    for (int j = 0; *ba != j; j = j + 1)
        ((char *)ba[1])[j] = 0;

    // free any stations on the stack at 0x1a0.
    unsigned *stk = (unsigned *)P(self, 0x1a0);
    if (stk != 0) {
        for (unsigned k = 0; k < *stk; k = k + 1) {
            Station *st = ((Station **)stk[1])[k];
            if (st != 0) {
                operator_delete(Station_dtor(st));
                ((int *)(*(int *)((char *)P(self, 0x1a0) + 4)))[k] = 0;
                stk = (unsigned *)P(self, 0x1a0);
            }
        }
    }

    Globals_resetHints();
    Galaxy *gal = *g_rg_galaxy;
    Galaxy_reset(gal);

    ba = (int *)P(self, 0x50);
    for (int j = 0; *ba != j; j = j + 1)
        ((char *)ba[1])[j] = 0;

    // array at 0x54: first 0xb0 entries = 1, rest = 0.
    unsigned *arr54 = (unsigned *)P(self, 0x54);
    char *d54 = (char *)arr54[1];
    for (int j = 0; j != 0xb0; j = j + 1)
        d54[j] = 1;
    for (unsigned k = 0xb0; k < *arr54; k = k + 1)
        d54[k] = 0;

    d54[0x3e] = 0;
    *(short *)(d54 + 0x3c) = 0;
    I(self, 0x34) = 0;
    I(self, 0xb8) = 0;
    // zero the two 16-byte ship-equipment-slot vectors.
    I(self, 0xe0) = 0; I(self, 0xe4) = 0; I(self, 0xe8) = 0; I(self, 0xec) = 0;
    I(self, 0xc0) = 0; I(self, 0xc4) = 0; I(self, 0xc8) = 0; I(self, 0xcc) = 0;
    I(self, 0xd0) = 0; I(self, 0xd4) = 0; I(self, 0xd8) = 0; I(self, 0xdc) = 0;

    // a few PC-relative scalar globals get zeroed.
    *(*g_rg_zeroSlotA) = 0;
    **g_rg_zeroSlotB   = 0;
    *(*g_rg_zeroSlotC) = 0;

    I(self, 0x160) = 0;
    I(self, 0x164) = 0;
    C(self, 0x108) = 0;
    I(self, 0x174) = 0;
    *(short *)(self + 0xf0) = 0;
    I(self, 0xf4) = -1;
    *(short *)(self + 0xf8) = 1;

    Achievements_init(*g_rg_ach, 1);

    // recreate the four int arrays at 0x40/0x3c/0x48/0x44 (length 0xe9).
    static const int off4[4] = {0x40, 0x3c, 0x48, 0x44};
    for (int q = 0; q < 4; q = q + 1) {
        if (P(self, off4[q]) != 0) {
            operator_delete(Array_int_dtor(P(self, off4[q])));
            I(self, off4[q]) = 0;
        }
    }
    for (int q = 0; q < 4; q = q + 1) {
        void *a = Status_opnew(0xc);
        Array_int_ctor(a);
        P(self, off4[q]) = a;
        ArraySetLength_int(0xe9, a);
    }
    {
        int *d40 = (int *)(*(int *)((char *)P(self, 0x40) + 4));
        int *d3c = (int *)(*(int *)((char *)P(self, 0x3c) + 4));
        int *d48 = (int *)(*(int *)((char *)P(self, 0x48) + 4));
        int *d44 = (int *)(*(int *)((char *)P(self, 0x44) + 4));
        for (int j = 0; j != 0xe9; j = j + 1) {
            d40[j] = 0; d3c[j] = 0; d48[j] = 0; d44[j] = 0;
        }
    }

    // bool array at 0x4c (length 4).
    if (P(self, 0x4c) != 0) {
        operator_delete(Array_bool_dtor(P(self, 0x4c)));
        I(self, 0x4c) = 0;
    }
    {
        void *a = Status_opnew(0xc);
        Array_bool_ctor(a);
        P(self, 0x4c) = a;
        ArraySetLength_bool(4, a);
        char *d = (char *)(*(int *)((char *)P(self, 0x4c) + 4));
        for (int j = 0; j != 4; j = j + 1) d[j] = 0;
    }

    // bool array at 0x58 (length 5).
    if (P(self, 0x58) != 0) {
        operator_delete(Array_bool_dtor(P(self, 0x58)));
        I(self, 0x58) = 0;
    }
    {
        void *a = Status_opnew(0xc);
        Array_bool_ctor(a);
        P(self, 0x58) = a;
        ArraySetLength_bool(5, a);
        char *d = (char *)(*(int *)((char *)P(self, 0x58) + 4));
        for (int j = 0; j != 5; j = j + 1) d[j] = 0;
    }

    // system visibilities at 0x38 from the galaxy systems.
    unsigned *systems = (unsigned *)Galaxy_getSystems(gal);
    void *vis = P(self, 0x38);
    for (unsigned k = 0; k < *systems; k = k + 1) {
        int v = SolarSystem_isVisible(((void **)systems[1])[k]);
        ((char *)(*(int *)((char *)vis + 4)))[k] = (char)v;
    }

    // count craftable blueprints.
    unsigned bpCount = 0;
    int *itHolder = *g_rg_itemTable;
    unsigned *items = (unsigned *)*itHolder;
    for (unsigned k = 0; k < *items; k = k + 1) {
        if (Item_getIngredients() != 0)
            bpCount = bpCount + 1;
    }

    if (P(self, 0x18) != 0) {
        ArrayReleaseClasses_BP(P(self, 0x18));
        if (P(self, 0x18) != 0)
            operator_delete(ArrayBP_dtor(P(self, 0x18)));
    }
    I(self, 0x18) = 0;
    if (bpCount != 0) {
        void *a = Status_opnew(0xc);
        ArrayBP_ctor(a);
        P(self, 0x18) = a;
        ArraySetLength_BP(bpCount, a);
        int idx = 0;
        for (unsigned k = 0; k < *items; k = k + 1) {
            if (Item_getIngredients() != 0) {
                void *bp = Status_opnew(0x2c);
                BluePrint_ctor(bp, k);
                ((void **)(*(int *)((char *)P(self, 0x18) + 4)))[idx] = bp;
                idx = idx + 1;
            }
        }
    }

    if (P(self, 0x1c) != 0) {
        ArrayReleaseClasses_PP(P(self, 0x1c));
        if (P(self, 0x1c) != 0)
            operator_delete(ArrayPP_dtor(P(self, 0x1c)));
    }
    I(self, 0x1c) = 0;

    Status_rg_loadAgents(this);
    Status_rg_loadWanted(this);

    // recreate Standing at 0x14.
    if (P(self, 0x14) != 0) {
        operator_delete(Standing_dtor((Standing *)P(self, 0x14)));
        I(self, 0x14) = 0;
    }
    Standing *st = (Standing *)Status_opnew(8);
    Standing_ctor(st);
    P(self, 0x14) = st;
    I(self, 0x7c) = -1;
    I(self, 0x80) = -1;
    I(self, 0x84) = 0;
    // missions[0] gets a base value from a status slot.
    *(int *)((*(int *)((char *)P(self, 0x198) + 4)) + 4) = **g_rg_statusSlotC;
    I(self, 0x1e8) = 0;

    void *mission = Status_opnew(0x78);
    Mission_ctor(mission, 4, 0, 0x4e);
    Status_rg_setCampaignMission(this, mission);

    int *slotB = *g_rg_statusSlotB;
    I(self, 0x194) = *(int *)(*(int *)((char *)P(self, 0x198) + 4));
    int newShip = Ship_makeShip(*(int *)((*(int *)(*slotB + 4)) + 0x28));
    Status_rg_setShip(this, newShip);
    Ship_priceDecline(*(Ship **)(self + 0x190));
    Status_rg_setStation(this, Galaxy_getStation((int)gal));
    Ship_setCargo(*(Ship **)(self + 0x190), 0);

    int (*makeItemB)(int) = g_rg_makeItemB;
    void (*addCargo)(int, int, int) = g_rg_addCargo;
    int shipObj = *(int *)(self + 0x190);
    int srcShip = *(int *)(*itHolder + 4);

    addCargo(shipObj, makeItemB(*(int *)(srcShip + 8)), 0);
    addCargo(shipObj, makeItemB(*(int *)(srcShip + 8)), 1);
    addCargo(shipObj, makeItemB(*(int *)(srcShip + 0xd8)), 0);
    addCargo(shipObj, makeItemB(*(int *)(srcShip + 0xec)), 1);
    addCargo(shipObj, makeItemB(*(int *)(srcShip + 0x148)), 2);
    addCargo(shipObj, makeItemB(*(int *)(srcShip + 0x124)), 3);
    addCargo(*(int *)((int)(*(int **)(*g_rg_statusSlotA)) + 0x190),
             Item_makeItem(*(int *)(srcShip + 0x90)), 0);

    if (C(srec, 0x35) != 0)
        ((char *)(*(int *)((char *)P(self, 0x38) + 4)))[0x19] = 1;

    I(self, 0x64) = Ship_getMaxHP(*(Ship **)(self + 0x190));
    I(self, 0x5c) = Ship_getMaxShieldHP();
    I(self, 0x60) = Ship_getMaxArmorHP(*(Ship **)(self + 0x190));
    I(self, 0x68) = 100;
    I(self, 0x150) = -1;
    I(self, 0x154) = -1;
    I(self, 0x158) = -1;
}
