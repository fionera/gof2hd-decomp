#include "Station.h"


extern "C" void ArrayReleaseClasses_Ship(void *arr);
extern "C" void *Array_Ship_dtor(void *arr);
extern "C" void Station_operator_delete(void *p);
extern "C" void *Array_Item_dtor(void *arr);
extern "C" void *Station_operator_new(uint32_t sz);
extern "C" void Array_Item_ctor(void *arr);
extern "C" void ArraySetLength_Item(uint32_t len, void *arr);
extern "C" void *Item_clone(Item *item);
extern "C" uint8_t Station_isDiscovered(Station *self);
extern "C" void Status_visitStation(Status *s);
extern "C" void Galaxy_setSystemVisited(Galaxy *g, int systemId);
extern "C" void String_copy_ctor(void *out, void *src, bool);
extern "C" void Array_Ship_ctor(void *arr);
extern "C" void ArraySetLength_Ship(uint32_t len, void *arr);
extern "C" void *Ship_clone(Ship *ship);
extern "C" void Station_arrayRemoveShip(Ship *ship, void *ships);
extern "C" char *Galaxy_getVisited(Galaxy *g);
extern "C" int Item_equals(Item *a, Item *b);
extern "C" int Item_getAmount(Item *item);
extern "C" void String_dtor(void *s);
extern "C" Station *Station_ctor(Station *self, void *name, int p3, int p4, int p5, int p6);
extern "C" void ArrayReleaseClasses_Ship(void *arr) __attribute__((nothrow));
extern "C" void ArrayReleaseClasses_Item(void *arr) __attribute__((nothrow));
extern "C" void *Array_Ship_dtor(void *arr) __attribute__((nothrow));
extern "C" void *Array_Item_dtor(void *arr) __attribute__((nothrow));
extern "C" void *Array_Agent_dtor(void *arr) __attribute__((nothrow));
extern "C" void Station_operator_delete(void *p) __attribute__((nothrow));
extern "C" Mission *Status_getCampaignMission(Status *s) __attribute__((nothrow));
extern "C" Mission *Status_getFreelanceMission(Status *s) __attribute__((nothrow));
extern "C" Agent *Mission_getAgent(Mission *m) __attribute__((nothrow));
extern "C" int Agent_isStoryAgent(Agent *a) __attribute__((nothrow));
extern "C" void *Agent_dtor(Agent *a) __attribute__((nothrow));
extern "C" void Station_baseDtor(void *self) __attribute__((nothrow));
extern "C" void ArrayReleaseClasses_Agent(void *arr);
extern "C" void *Array_Agent_dtor(void *arr);
extern "C" int Ship_getIndex(Ship *ship);
extern "C" int Item_getIndex(Item *item);
extern "C" int Ship_equals(Ship *a, Ship *b);

// ---- removeShips_a6c74.cpp ----
struct Ship;

extern "C" void Station_removeShips(Station *self)
{
    void *arr = F<void *>(self, 0x2c);
    if (arr != 0) {
        ArrayReleaseClasses_Ship(arr);
        void *a2 = F<void *>(self, 0x2c);
        if (a2 != 0)
            Station_operator_delete(Array_Ship_dtor(a2));
    }
    F<void *>(self, 0x2c) = 0;
}

// ---- stationHasHiddenBlueprint_a700c.cpp ----
static const int kHiddenBlueprints[5] = { 0, 0, 0, 0, 0 };
extern char **const gHiddenBlueprintSingleton __attribute__((visibility("hidden")));

// Station::stationHasHiddenBlueprint(bool) — this in r0, flag in r1.
extern "C" uint32_t Station_stationHasHiddenBlueprint(Station *self, bool ignoreFound)
{
    char *base = *gHiddenBlueprintSingleton;
    uint32_t i = 0;
    while (true) {
        if (i > 4)
            return 0;
        if (kHiddenBlueprints[i] == F<int>(self, 0xc)) {
            if (ignoreFound)
                return 1;
            char *flags = *(char **)(*(char **)(base + 0x58) + 4);
            if (flags[i] == 0)
                return 1;
        }
        i++;
    }
}

// ---- stationHasPirateBase_a6fa4.cpp ----
static const int kPirateStations[4] = { 0, 0, 0, 0 };
extern char **const gPirateBaseSingleton __attribute__((visibility("hidden")));

// Station::stationHasPirateBase() — this in r0.
extern "C" uint32_t Station_stationHasPirateBase(Station *self)
{
    char *base = *gPirateBaseSingleton;       // r12 = *(*(global))
    uint32_t i = 0;
    while (true) {
        if (i > 3)
            return 0;
        if (kPirateStations[i] == F<int>(self, 0xc)) {
            char *flags = *(char **)(*(char **)(base + 0x4c) + 4);
            if (flags[i] == 0)
                return 1;
        }
        i++;
    }
}

// ---- setItems_a6d5e.cpp ----
struct Item;

// Station::setItems(Array<Item*>*, bool) — this in r0, items in r1, deep in r2.
extern "C" void Station_setItems(Station *self, uint32_t *items, bool deep)
{
    void *old = F<void *>(self, 0x28);
    if (old != 0)
        Station_operator_delete(Array_Item_dtor(old));
    F<void *>(self, 0x28) = 0;
    if (items == 0 || !deep) {
        F<uint32_t *>(self, 0x28) = items;
    } else {
        void *na = Station_operator_new(0xc);
        Array_Item_ctor(na);
        F<void *>(self, 0x28) = na;
        ArraySetLength_Item(items[0], na);
        for (uint32_t i = 0; i < items[0]; i++) {
            void *cloned = Item_clone(((Item **)items[1])[i]);
            ((void **)((uint32_t *)F<uint32_t *>(self, 0x28))[1])[i] = cloned;
        }
    }
}

// ---- equals_a70ec.cpp ----
extern "C" bool Station_equals(Station *self, Station *other)
{
    if (other != 0)
        return F<int>(self, 0xc) == F<int>(other, 0xc);
    return false;
}

// ---- visit_a6c2c.cpp ----
struct Status;
struct Galaxy;
// tail-called veneer: marks the system visited, given (Galaxy*, systemId)

extern Status **const gStatusSingleton __attribute__((visibility("hidden")));
extern Galaxy **const gGalaxyVisit __attribute__((visibility("hidden")));

extern "C" void Station_visit(Station *self)
{
    if (Station_isDiscovered(self) != 0)
        return;
    F<uint8_t>(self, 0x1c) = 1;
    Status_visitStation(*gStatusSingleton);
    Galaxy_setSystemVisited(*gGalaxyVisit, F<int>(self, 0xc));
}

// ---- getName_a6bce.cpp ----
struct RetStr { uint32_t a, b, c; };

// Station::getName() -> String by value (sret in r0, this in r1).
extern "C" RetStr Station_getName(Station *self)
{
    RetStr r;
    String_copy_ctor(&r, self, false);
    return r;
}

// ---- setShips_a6ce6.cpp ----
struct Ship;

// Station::setShips(Array<Ship*>*, bool) — this in r0, ships in r1, deep in r2.
extern "C" void Station_setShips(Station *self, uint32_t *ships, bool deep)
{
    void *old = F<void *>(self, 0x2c);
    if (old != 0) {
        ArrayReleaseClasses_Ship(old);
        void *o2 = F<void *>(self, 0x2c);
        if (o2 != 0)
            Station_operator_delete(Array_Ship_dtor(o2));
    }
    F<void *>(self, 0x2c) = 0;
    if (ships == 0 || !deep) {
        F<uint32_t *>(self, 0x2c) = ships;
    } else {
        void *na = Station_operator_new(0xc);
        Array_Ship_ctor(na);
        F<void *>(self, 0x2c) = na;
        ArraySetLength_Ship(ships[0], na);
        for (uint32_t i = 0; i < ships[0]; i++) {
            void *cloned = Ship_clone(((Ship **)ships[1])[i]);
            ((void **)((uint32_t *)F<uint32_t *>(self, 0x2c))[1])[i] = cloned;
        }
    }
}

// ---- isAttackedByAliens_a6bec.cpp ----
// Global slot holding a pointer P; *P -> Q; *(Q+0x80) is the compared value.
extern int **const gAlienAttackSingleton __attribute__((visibility("hidden")));

extern "C" bool Station_isAttackedByAliens(Station *self)
{
    return F<int>(self, 0xc) == *(int *)((char *)(*gAlienAttackSingleton) + 0x80);
}

// ---- removeShip_a6c94.cpp ----
struct Ship;
// ArrayRemove<Ship*>(Ship* ship, Array* ships) — tail-called veneer.

// Station::removeShip(Ship*) — this in r0, ship in r1.
extern "C" void Station_removeShip(Station *self, Ship *ship)
{
    void *ships = F<void *>(self, 0x2c);
    if (ships == 0)
        return;
    Station_arrayRemoveShip(ship, ships);
}

// ---- getHiddenBlueprintIndex_a7050.cpp ----
static const int kHiddenBlueprints[5] = { 0, 0, 0, 0, 0 };

extern "C" uint32_t Station_getHiddenBlueprintIndex(Station *self)
{
    uint32_t i = 0;
    while (true) {
        if (i > 4)
            return 0xffffffff;
        if (kHiddenBlueprints[i] == F<int>(self, 0xc))
            return i;
        i++;
    }
}

// ---- isDiscovered_a6c10.cpp ----
struct Galaxy;

// Global slot -> P; *P -> the Galaxy* singleton.
extern Galaxy **const gGalaxySingleton __attribute__((visibility("hidden")));

extern "C" uint8_t Station_isDiscovered(Station *self)
{
    char *visited = Galaxy_getVisited(*gGalaxySingleton);
    return visited[F<int>(self, 0xc)];
}

// ---- setAttackedFriends_a6bde.cpp ----
extern "C" void Station_setAttackedFriends(Station *self, bool v)
{
    F<uint8_t>(self, 0x24) = v;
}

// ---- hasAttackedFriends_a6be4.cpp ----
extern "C" uint8_t Station_hasAttackedFriends(Station *self)
{
    return F<uint8_t>(self, 0x24);
}

// ---- getPirateStationIndex_a6fe4.cpp ----
static const int kPirateStations[4] = { 0, 0, 0, 0 };

extern "C" uint32_t Station_getPirateStationIndex(Station *self)
{
    uint32_t i = 0;
    while (true) {
        if (i > 3)
            return 0xffffffff;
        if (kPirateStations[i] == F<int>(self, 0xc))
            return i;
        i++;
    }
}

// ---- addItem_a6eb6.cpp ----
struct Item;
extern "C" void Item_addAmount(Item *found, int amount);  // tail-called veneer
extern "C" void ArrayAdd_Item(Item *item, void *arr);     // tail-called veneer

// Station::addItem(Item*) — this in r0, item in r1.
extern "C" void Station_addItem(Station *self, Item *item)
{
    uint32_t *arr = F<uint32_t *>(self, 0x28);
    if (arr == 0) {
        arr = (uint32_t *)Station_operator_new(0xc);
        Array_Item_ctor(arr);
        F<uint32_t *>(self, 0x28) = arr;
    } else {
        uint32_t n = arr[0];
        if (n != 0) {
            for (uint32_t i = 0; i < n; i++) {
                if (Item_equals(((Item **)arr[1])[i], item) != 0) {
                    if ((int)i < 0)
                        break;
                    uint32_t *cur = F<uint32_t *>(self, 0x28);
                    Item *found = ((Item **)cur[1])[i];
                    Item_addAmount(found, Item_getAmount(item));
                    return;
                }
                arr = F<uint32_t *>(self, 0x28);
                n = arr[0];
            }
        }
    }
    ArrayAdd_Item(item, arr);
}

// ---- clone_a7078.cpp ----
// Station::clone() — this in r0, returns a new Station copy.
extern "C" Station *Station_clone(Station *self)
{
    Station *n = (Station *)Station_operator_new(0x34);
    char tmp[12];
    String_copy_ctor(tmp, self, false);
    Station_ctor(n, tmp, F<int>(self, 0xc), F<int>(self, 0x10),
                 F<int>(self, 0x20), F<int>(self, 0x18));
    String_dtor(tmp);
    return n;
}

// ---- _Station_a69f4.cpp ----
// Station::~Station() — real C++ destructor so the demangled symbol contains "~Station".

typedef unsigned int uint32_t;

struct Ship; struct Item; struct Agent; struct Mission; struct Status;



// base destructor (String member dtor), tail-called directly.

// Status singleton: global slot holds P (kept in a reg); *P is reloaded per call.
extern Status **const gStatusForDtor __attribute__((visibility("hidden")));
#define STATUS (*gStatusForDtor)

template <class T>
struct Array { uint32_t length; T *data; uint32_t cap; };

struct Station {
    unsigned char pad_0[0x28];
    Array<Item *> *items;   // 0x28
    Array<Ship *> *ships;   // 0x2c
    Array<Agent *> *agents; // 0x30
    ~Station() noexcept(false);
};

__attribute__((minsize)) Station::~Station() noexcept(false)
{
    if (this->ships != 0) {
        ArrayReleaseClasses_Ship(this->ships);
        if (this->ships != 0)
            Station_operator_delete(Array_Ship_dtor(this->ships));
        this->ships = 0;
    }
    if (this->items != 0) {
        ArrayReleaseClasses_Item(this->items);
        if (this->items != 0)
            Station_operator_delete(Array_Item_dtor(this->items));
        this->items = 0;
    }
    Array<Agent *> *agents = this->agents;
    if (agents != 0) {
        for (uint32_t i = 0; i < agents->length; i++) {
            Agent *a = agents->data[i];
            Agent *campA = Status_getCampaignMission(STATUS) == 0
                               ? (Agent *)0
                               : Mission_getAgent(Status_getCampaignMission(STATUS));
            Agent *freeA = Status_getFreelanceMission(STATUS) == 0
                               ? (Agent *)0
                               : Mission_getAgent(Status_getFreelanceMission(STATUS));
            if (a != 0 && a != campA && a != freeA && Agent_isStoryAgent(a) == 0)
                Station_operator_delete(Agent_dtor(a));
            agents = this->agents;
        }
        if (agents != 0)
            Station_operator_delete(Array_Agent_dtor(agents));
        this->agents = 0;
    }
    Station_baseDtor(this);
}

// ---- setAgents_a6dce.cpp ----
struct Agent;

extern "C" void Station_setAgents(Station *self, void *agents)
{
    void *cur = F<void *>(self, 0x30);
    if (cur != agents) {
        if (cur != 0) {
            ArrayReleaseClasses_Agent(cur);
            void *a2 = F<void *>(self, 0x30);
            if (a2 != 0)
                Station_operator_delete(Array_Agent_dtor(a2));
        }
        F<void *>(self, 0x30) = agents;
    }
}

// ---- Station_a6978.cpp ----
extern "C" void *String_default_ctor(void *s);                 // String::String() -> this
extern "C" void String_from_cstr(void *out, const char *s, bool); // String::String(const char*, bool)
extern "C" void String_assign(void *dst, void *src);           // operator=(String*, String*)

extern const char kStationDefaultName[] __attribute__((visibility("hidden")));

// Station::Station() — default ctor.
extern "C" void Station_ctor_default(Station *self)
{
    String_default_ctor(self);
    char tmp[12];
    String_from_cstr(tmp, kStationDefaultName, false);
    String_assign(self, tmp);
    String_dtor(tmp);
    F<int>(self, 0xc) = -1;
    F<int>(self, 0x10) = -1;
    F<int>(self, 0x20) = 0;
    F<int>(self, 0x18) = 0;
    F<uint8_t>(self, 0x1c) = 0;
    F<uint8_t>(self, 0x24) = 0;
    F<int>(self, 0x28) = 0;
    F<int>(self, 0x2c) = 0;
    F<int>(self, 0x30) = 0;
}

// ---- hasShip_a6e7c.cpp ----
struct Ship;

// Station::hasShip(int) — this in r0, index in r1.
extern "C" uint32_t Station_hasShip(Station *self, int index)
{
    uint32_t *arr = F<uint32_t *>(self, 0x2c);
    if (arr != 0) {
        for (uint32_t i = 0; i < arr[0]; i++) {
            Ship *sh = ((Ship **)arr[1])[i];
            if (sh != 0) {
                if (Ship_getIndex(sh) == index)
                    return 1;
                arr = F<uint32_t *>(self, 0x2c);
            }
        }
    }
    return 0;
}

// ---- hasItem_a6e42.cpp ----
struct Item;

// Station::hasItem(int) — this in r0, index in r1.
extern "C" uint32_t Station_hasItem(Station *self, int index)
{
    uint32_t *arr = F<uint32_t *>(self, 0x28);
    if (arr != 0) {
        for (uint32_t i = 0; i < arr[0]; i++) {
            Item *it = ((Item **)arr[1])[i];
            if (it != 0) {
                if (Item_getIndex(it) == index)
                    return 1;
                arr = F<uint32_t *>(self, 0x28);
            }
        }
    }
    return 0;
}

// ---- addShip_a6f36.cpp ----
struct Ship;
extern "C" void ArrayAdd_Ship(Ship *ship, void *arr);   // tail-called veneer

// Station::addShip(Ship*) — this in r0, ship in r1.
extern "C" void Station_addShip(Station *self, Ship *ship)
{
    uint32_t *arr = F<uint32_t *>(self, 0x2c);
    if (arr == 0) {
        arr = (uint32_t *)Station_operator_new(0xc);
        Array_Ship_ctor(arr);
        F<uint32_t *>(self, 0x2c) = arr;
    } else {
        uint32_t n = arr[0];
        if (n != 0) {
            for (uint32_t i = 0; i < n; i++) {
                if (Ship_equals(((Ship **)arr[1])[i], ship) != 0) {
                    if ((int)i >= 0)
                        return;
                    arr = F<uint32_t *>(self, 0x2c);
                    break;
                }
                arr = F<uint32_t *>(self, 0x2c);
                n = arr[0];
            }
        }
    }
    ArrayAdd_Ship(ship, arr);
}

// ---- Station_a692c.cpp ----
extern "C" void *String_default_ctor(void *s);         // String::String() -> this
extern "C" void String_assign(void *dst, void *src);   // operator=(String*, String*)

// Station::Station(String, int, int, int, int)
// r0=this, r1=String* param, r2=p3, r3=p4, [stack]=p5, p6.
extern "C" Station *Station_ctor(Station *self, void *name,
                                 int p3, int p4, int p5, int p6)
{
    void *s = String_default_ctor(self);
    String_assign(s, name);
    F<int>(self, 0xc) = p3;
    F<int>(self, 0x10) = p4;
    F<int>(self, 0x20) = p5;
    F<int>(self, 0x18) = p6;
    F<uint8_t>(self, 0x1c) = 0;
    F<uint8_t>(self, 0x24) = 0;
    F<int>(self, 0x28) = 0;
    F<int>(self, 0x2c) = 0;
    F<int>(self, 0x30) = 0;
    return self;
}
