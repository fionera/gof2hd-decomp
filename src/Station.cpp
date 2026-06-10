#include "gof2/Station.h"
#include "gof2/Agent.h"
#include "gof2/String.h"


extern "C" void *Station_operator_new(uint32_t sz);
extern "C" void Array_Item_ctor(void *arr);
extern "C" void ArraySetLength_Item(uint32_t len, void *arr);
extern "C" void *Item_clone(Item *item);
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
extern "C" void ArrayReleaseClasses_Ship(void *arr) __attribute__((nothrow));
extern "C" void ArrayReleaseClasses_Item(void *arr) __attribute__((nothrow));
extern "C" void *Array_Ship_dtor(void *arr) __attribute__((nothrow));
extern "C" void *Array_Item_dtor(void *arr) __attribute__((nothrow));
extern "C" void *Array_Agent_dtor(void *arr) __attribute__((nothrow));
extern "C" void Station_operator_delete(void *p) __attribute__((nothrow));
extern "C" Mission *Status_getCampaignMission(Status *s) __attribute__((nothrow));
extern "C" Mission *Status_getFreelanceMission(Status *s) __attribute__((nothrow));
extern "C" Agent *Mission_getAgent(Mission *m) __attribute__((nothrow));
extern "C" void *Agent_dtor(Agent *a) __attribute__((nothrow));
extern "C" void Station_baseDtor(void *self) __attribute__((nothrow));
extern "C" void ArrayReleaseClasses_Agent(void *arr) __attribute__((nothrow));
extern "C" int Ship_getIndex(Ship *ship);
extern "C" int Item_getIndex(Item *item);
extern "C" int Ship_equals(Ship *a, Ship *b);

// ---- removeShips_a6c74.cpp ----
struct Ship;

void Station::removeShips() {
    Station *self = this;
    void *arr = self->field_0x2c;
    if (arr != 0) {
        ArrayReleaseClasses_Ship(arr);
        void *a2 = self->field_0x2c;
        if (a2 != 0)
            Station_operator_delete(Array_Ship_dtor(a2));
    }
    self->field_0x2c = 0;
}

// ---- stationHasHiddenBlueprint_a700c.cpp ----
static const int kHiddenBlueprints[5] = { 0, 0, 0, 0, 0 };
extern char **const gHiddenBlueprintSingleton __attribute__((visibility("hidden")));

// Station::stationHasHiddenBlueprint(bool) — this in r0, flag in r1.
uint32_t Station::stationHasHiddenBlueprint(bool ignoreFound) {
    Station *self = this;
    char *base = *gHiddenBlueprintSingleton;
    uint32_t i = 0;
    while (true) {
        if (i > 4)
            return 0;
        if (kHiddenBlueprints[i] == self->field_0xc) {
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
uint32_t Station::stationHasPirateBase() {
    Station *self = this;
    char *base = *gPirateBaseSingleton;       // r12 = *(*(global))
    uint32_t i = 0;
    while (true) {
        if (i > 3)
            return 0;
        if (kPirateStations[i] == self->field_0xc) {
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
void Station::setItems(uint32_t *items, bool deep) {
    Station *self = this;
    void *old = self->field_0x28;
    if (old != 0)
        Station_operator_delete(Array_Item_dtor(old));
    self->field_0x28 = 0;
    if (items == 0 || !deep) {
        self->field_0x28 = items;
    } else {
        void *na = Station_operator_new(0xc);
        Array_Item_ctor(na);
        self->field_0x28 = na;
        ArraySetLength_Item(items[0], na);
        for (uint32_t i = 0; i < items[0]; i++) {
            void *cloned = Item_clone(((Item **)items[1])[i]);
            ((void **)((uint32_t *)self->field_0x28)[1])[i] = cloned;
        }
    }
}

// ---- equals_a70ec.cpp ----
bool Station::equals(Station *other) {
    Station *self = this;
    if (other != 0)
        return self->field_0xc == other->field_0xc;
    return false;
}

// ---- visit_a6c2c.cpp ----
struct Status;
struct Galaxy;
// tail-called veneer: marks the system visited, given (Galaxy*, systemId)

extern Status **const gStatusSingleton __attribute__((visibility("hidden")));
extern Galaxy **const gGalaxyVisit __attribute__((visibility("hidden")));

void Station::visit() {
    Station *self = this;
    if (((Station *)(self))->isDiscovered() != 0)
        return;
    self->field_0x1c = 1;
    Status_visitStation(*gStatusSingleton);
    Galaxy_setSystemVisited(*gGalaxyVisit, self->field_0xc);
}

// ---- getName_a6bce.cpp ----
// RetStr is declared in gof2/Station.h (via gof2/Agent.h).

// Station::getName() -> String by value (sret in r0, this in r1).
RetStr Station::getName() {
    Station *self = this;
    RetStr r;
    String_copy_ctor(&r, self, false);
    return r;
}

// ---- setShips_a6ce6.cpp ----
struct Ship;

// Station::setShips(Array<Ship*>*, bool) — this in r0, ships in r1, deep in r2.
void Station::setShips(uint32_t *ships, bool deep) {
    Station *self = this;
    void *old = self->field_0x2c;
    if (old != 0) {
        ArrayReleaseClasses_Ship(old);
        void *o2 = self->field_0x2c;
        if (o2 != 0)
            Station_operator_delete(Array_Ship_dtor(o2));
    }
    self->field_0x2c = 0;
    if (ships == 0 || !deep) {
        self->field_0x2c = ships;
    } else {
        void *na = Station_operator_new(0xc);
        Array_Ship_ctor(na);
        self->field_0x2c = na;
        ArraySetLength_Ship(ships[0], na);
        for (uint32_t i = 0; i < ships[0]; i++) {
            void *cloned = Ship_clone(((Ship **)ships[1])[i]);
            ((void **)((uint32_t *)self->field_0x2c)[1])[i] = cloned;
        }
    }
}

// ---- isAttackedByAliens_a6bec.cpp ----
// Global slot holding a pointer P; *P -> Q; *(Q+0x80) is the compared value.
extern int **const gAlienAttackSingleton __attribute__((visibility("hidden")));

bool Station::isAttackedByAliens() {
    Station *self = this;
    return self->field_0xc == *(int *)((char *)(*gAlienAttackSingleton) + 0x80);
}

// ---- removeShip_a6c94.cpp ----
struct Ship;
// ArrayRemove<Ship*>(Ship* ship, Array* ships) — tail-called veneer.

// Station::removeShip(Ship*) — this in r0, ship in r1.
void Station::removeShip(Ship *ship) {
    Station *self = this;
    void *ships = self->field_0x2c;
    if (ships == 0)
        return;
    Station_arrayRemoveShip(ship, ships);
}

// ---- getHiddenBlueprintIndex_a7050.cpp ----
uint32_t Station::getHiddenBlueprintIndex() {
    Station *self = this;
    uint32_t i = 0;
    while (true) {
        if (i > 4)
            return 0xffffffff;
        if (kHiddenBlueprints[i] == self->field_0xc)
            return i;
        i++;
    }
}

// ---- isDiscovered_a6c10.cpp ----
struct Galaxy;

// Global slot -> P; *P -> the Galaxy* singleton.
extern Galaxy **const gGalaxySingleton __attribute__((visibility("hidden")));

uint8_t Station::isDiscovered() {
    Station *self = this;
    char *visited = Galaxy_getVisited(*gGalaxySingleton);
    return visited[self->field_0xc];
}

// ---- setAttackedFriends_a6bde.cpp ----
void Station::setAttackedFriends(bool v) {
    Station *self = this;
    self->field_0x24 = v;
}

// ---- hasAttackedFriends_a6be4.cpp ----
uint8_t Station::hasAttackedFriends() {
    Station *self = this;
    return self->field_0x24;
}

// ---- getPirateStationIndex_a6fe4.cpp ----
uint32_t Station::getPirateStationIndex() {
    Station *self = this;
    uint32_t i = 0;
    while (true) {
        if (i > 3)
            return 0xffffffff;
        if (kPirateStations[i] == self->field_0xc)
            return i;
        i++;
    }
}

// ---- addItem_a6eb6.cpp ----
struct Item;
extern "C" void Item_addAmount(Item *found, int amount);  // tail-called veneer
extern "C" void ArrayAdd_Item(Item *item, void *arr);     // tail-called veneer

// Station::addItem(Item*) — this in r0, item in r1.
void Station::addItem(Item *item) {
    Station *self = this;
    uint32_t *arr = (uint32_t *)self->field_0x28;
    if (arr == 0) {
        arr = (uint32_t *)Station_operator_new(0xc);
        Array_Item_ctor(arr);
        self->field_0x28 = arr;
    } else {
        uint32_t n = arr[0];
        if (n != 0) {
            for (uint32_t i = 0; i < n; i++) {
                if (Item_equals(((Item **)arr[1])[i], item) != 0) {
                    if ((int)i < 0)
                        break;
                    uint32_t *cur = (uint32_t *)self->field_0x28;
                    Item *found = ((Item **)cur[1])[i];
                    Item_addAmount(found, Item_getAmount(item));
                    return;
                }
                arr = (uint32_t *)self->field_0x28;
                n = arr[0];
            }
        }
    }
    ArrayAdd_Item(item, arr);
}

// ---- clone_a7078.cpp ----
// Station::clone() — this in r0, returns a new Station copy.
Station * Station::clone() {
    Station *self = this;
    Station *n = (Station *)Station_operator_new(0x34);
    char tmp[12];
    String_copy_ctor(tmp, self, false);
    ((Station *)(n))->ctor(tmp, self->field_0xc, self->field_0x10, self->field_0x20, self->field_0x18);
    ((String *)(tmp))->dtor();
    return n;
}

// ---- _Station_a69f4.cpp ----
// Station::~Station() — real C++ destructor so the demangled symbol contains "~Station".

// Engine container layout: { uint32_t length; T* data; uint32_t cap; }
struct StationArray { uint32_t length; void **data; uint32_t cap; };

// Status singleton: global slot holds P (kept in a reg); *P is reloaded per call.
extern Status **const gStatusForDtor __attribute__((visibility("hidden")));
#define STATUS (*gStatusForDtor)

// Station::~Station()
void Station::dtor() {
    Station *self = this;
    if (self->field_0x2c != 0) {
        ArrayReleaseClasses_Ship(self->field_0x2c);
        if (self->field_0x2c != 0)
            Station_operator_delete(Array_Ship_dtor(self->field_0x2c));
        self->field_0x2c = 0;
    }
    if (self->field_0x28 != 0) {
        ArrayReleaseClasses_Item(self->field_0x28);
        if (self->field_0x28 != 0)
            Station_operator_delete(Array_Item_dtor(self->field_0x28));
        self->field_0x28 = 0;
    }
    StationArray *agents = (StationArray *)self->field_0x30;
    if (agents != 0) {
        for (uint32_t i = 0; i < agents->length; i++) {
            Agent *a = (Agent *)agents->data[i];
            Agent *campA = Status_getCampaignMission(STATUS) == 0
                               ? (Agent *)0
                               : Mission_getAgent(Status_getCampaignMission(STATUS));
            Agent *freeA = Status_getFreelanceMission(STATUS) == 0
                               ? (Agent *)0
                               : Mission_getAgent(Status_getFreelanceMission(STATUS));
            if (a != 0 && a != campA && a != freeA && ((Agent *)(a))->isStoryAgent() == 0)
                Station_operator_delete(Agent_dtor(a));
            agents = (StationArray *)self->field_0x30;
        }
        if (agents != 0)
            Station_operator_delete(Array_Agent_dtor(agents));
        self->field_0x30 = 0;
    }
    Station_baseDtor(self);
}

// ---- setAgents_a6dce.cpp ----
struct Agent;

void Station::setAgents(void *agents) {
    Station *self = this;
    void *cur = self->field_0x30;
    if (cur != agents) {
        if (cur != 0) {
            ArrayReleaseClasses_Agent(cur);
            void *a2 = self->field_0x30;
            if (a2 != 0)
                Station_operator_delete(Array_Agent_dtor(a2));
        }
        self->field_0x30 = agents;
    }
}

// ---- Station_a6978.cpp ----
extern "C" void *String_default_ctor(void *s);                 // String::String() -> this
extern "C" void String_from_cstr(void *out, const char *s, bool); // String::String(const char*, bool)
// operator=(String*, String*)

extern const char kStationDefaultName[] __attribute__((visibility("hidden")));

// Station::Station() — default ctor.
void Station::ctor_default() {
    Station *self = this;
    String_default_ctor(self);
    char tmp[12];
    String_from_cstr(tmp, kStationDefaultName, false);
    ((String *)(self))->assign((String *)tmp);
    ((String *)(tmp))->dtor();
    self->field_0xc = -1;
    self->field_0x10 = -1;
    self->field_0x20 = 0;
    self->field_0x18 = 0;
    self->field_0x1c = 0;
    self->field_0x24 = 0;
    self->field_0x28 = 0;
    self->field_0x2c = 0;
    self->field_0x30 = 0;
}

// ---- hasShip_a6e7c.cpp ----
struct Ship;

// Station::hasShip(int) — this in r0, index in r1.
uint32_t Station::hasShip(int index) {
    Station *self = this;
    uint32_t *arr = (uint32_t *)self->field_0x2c;
    if (arr != 0) {
        for (uint32_t i = 0; i < arr[0]; i++) {
            Ship *sh = ((Ship **)arr[1])[i];
            if (sh != 0) {
                if (Ship_getIndex(sh) == index)
                    return 1;
                arr = (uint32_t *)self->field_0x2c;
            }
        }
    }
    return 0;
}

// ---- hasItem_a6e42.cpp ----
struct Item;

// Station::hasItem(int) — this in r0, index in r1.
uint32_t Station::hasItem(int index) {
    Station *self = this;
    uint32_t *arr = (uint32_t *)self->field_0x28;
    if (arr != 0) {
        for (uint32_t i = 0; i < arr[0]; i++) {
            Item *it = ((Item **)arr[1])[i];
            if (it != 0) {
                if (Item_getIndex(it) == index)
                    return 1;
                arr = (uint32_t *)self->field_0x28;
            }
        }
    }
    return 0;
}

// ---- addShip_a6f36.cpp ----
struct Ship;
extern "C" void ArrayAdd_Ship(Ship *ship, void *arr);   // tail-called veneer

// Station::addShip(Ship*) — this in r0, ship in r1.
void Station::addShip(Ship *ship) {
    Station *self = this;
    uint32_t *arr = (uint32_t *)self->field_0x2c;
    if (arr == 0) {
        arr = (uint32_t *)Station_operator_new(0xc);
        Array_Ship_ctor(arr);
        self->field_0x2c = arr;
    } else {
        uint32_t n = arr[0];
        if (n != 0) {
            for (uint32_t i = 0; i < n; i++) {
                if (Ship_equals(((Ship **)arr[1])[i], ship) != 0) {
                    if ((int)i >= 0)
                        return;
                    arr = (uint32_t *)self->field_0x2c;
                    break;
                }
                arr = (uint32_t *)self->field_0x2c;
                n = arr[0];
            }
        }
    }
    ArrayAdd_Ship(ship, arr);
}

// ---- Station_a692c.cpp ----
extern "C" void *String_default_ctor(void *s);         // String::String() -> this
// operator=(String*, String*)

// Station::Station(String, int, int, int, int)
// r0=this, r1=String* param, r2=p3, r3=p4, [stack]=p5, p6.
Station * Station::ctor(void *name, int p3, int p4, int p5, int p6) {
    Station *self = this;
    void *s = String_default_ctor(self);
    ((String *)(s))->assign((String *)name);
    self->field_0xc = p3;
    self->field_0x10 = p4;
    self->field_0x20 = p5;
    self->field_0x18 = p6;
    self->field_0x1c = 0;
    self->field_0x24 = 0;
    self->field_0x28 = 0;
    self->field_0x2c = 0;
    self->field_0x30 = 0;
    return self;
}
