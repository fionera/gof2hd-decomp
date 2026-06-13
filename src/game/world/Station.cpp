#include "gof2/game/world/Station.h"
#include "gof2/game/ship/Ship.h"
#include "gof2/game/world/Galaxy.h"
#include "gof2/game/mission/Mission.h"
#include "gof2/game/mission/Item.h"
#include "gof2/game/mission/Status.h"
#include "gof2/game/ship/Agent.h"
#include "gof2/game/core/String.h"

// NOTE: Station's name is an AbyssEngine::String passed/returned BY VALUE through the engine's
// 12-byte RetStr/String12 aggregate ABI (getName() returns RetStr, callers do `*(RetStr*)tmp = ...`).
// That trivially-copied 12-byte aggregate does not match our 24-byte std::u16string-backed String,
// so these by-value-String copy/format entry points are kept as documented engine externs rather
// than rewritten to std::u16string methods (doing so would require rewriting the whole RetStr ABI).
extern "C" void String_copy_ctor(void *out, void *src, bool);
extern "C" void *Agent_dtor(Agent *a) __attribute__((nothrow));

void Station::removeShips() {
    Array<Ship*> *arr = this->ships;
    if (arr != 0) {
        for (Ship *s : *arr) delete s;
        arr->clear();
        if (this->ships != 0)
            delete this->ships;
    }
    this->ships = 0;
}

static const int kHiddenBlueprints[5] = { 0, 0, 0, 0, 0 };
extern char **const gHiddenBlueprintSingleton __attribute__((visibility("hidden")));

// Station::stationHasHiddenBlueprint(bool) — this in r0, flag in r1.
uint32_t Station::stationHasHiddenBlueprint(bool ignoreFound) {
    char *base = *gHiddenBlueprintSingleton;
    uint32_t i = 0;
    while (true) {
        if (i > 4)
            return 0;
        if (kHiddenBlueprints[i] == this->index) {
            if (ignoreFound)
                return 1;
            char *flags = *(char **)(*(char **)(base + 0x58) + 4);
            if (flags[i] == 0)
                return 1;
        }
        i++;
    }
}

static const int kPirateStations[4] = { 0, 0, 0, 0 };
extern char **const gPirateBaseSingleton __attribute__((visibility("hidden")));

// Station::stationHasPirateBase() — this in r0.
uint32_t Station::stationHasPirateBase() {
    char *base = *gPirateBaseSingleton;       // r12 = *(*(global))
    uint32_t i = 0;
    while (true) {
        if (i > 3)
            return 0;
        if (kPirateStations[i] == this->index) {
            char *flags = *(char **)(*(char **)(base + 0x4c) + 4);
            if (flags[i] == 0)
                return 1;
        }
        i++;
    }
}

struct Item;

// Station::setItems(Array<Item*>*, bool) — this in r0, items in r1, deep in r2.
void Station::setItems(uint32_t *items, bool deep) {
    if (this->items != 0)
        delete this->items;
    this->items = 0;
    Array<Item*> *src = (Array<Item*> *)items;
    if (src == 0 || !deep) {
        this->items = src;
    } else {
        Array<Item*> *na = new Array<Item*>();
        this->items = na;
        na->resize(src->size());
        for (uint32_t i = 0; i < src->size(); i++) {
            (*na)[i] = (*src)[i]->clone();
        }
    }
}

bool Station::equals(Station *other) {
    if (other != 0)
        return this->index == other->index;
    return false;
}

struct Status;
struct Galaxy;
// tail-called veneer: marks the system visited, given (Galaxy*, systemId)

extern Status **const gStatusSingleton __attribute__((visibility("hidden")));
extern Galaxy **const gGalaxyVisit __attribute__((visibility("hidden")));

void Station::visit() {
    if (((Station *)(this))->isDiscovered() != 0)
        return;
    this->visited = 1;
    ((Status *)(*gStatusSingleton))->visitStation();
    ((Galaxy *)(*gGalaxyVisit))->setSystemVisited(this->index);
}

// RetStr is declared in gof2/Station.h (via gof2/Agent.h).

// Station::getName() -> String by value (sret in r0, this in r1).
RetStr Station::getName() {
    RetStr r;
    String_copy_ctor(&r, this, false);
    return r;
}

struct Ship;

// Station::setShips(Array<Ship*>*, bool) — this in r0, ships in r1, deep in r2.
void Station::setShips(uint32_t *ships, bool deep) {
    if (this->ships != 0) {
        for (Ship *s : *this->ships) delete s;
        this->ships->clear();
        if (this->ships != 0)
            delete this->ships;
    }
    this->ships = 0;
    Array<Ship*> *src = (Array<Ship*> *)ships;
    if (src == 0 || !deep) {
        this->ships = src;
    } else {
        Array<Ship*> *na = new Array<Ship*>();
        this->ships = na;
        na->resize(src->size());
        for (uint32_t i = 0; i < src->size(); i++) {
            (*na)[i] = (*src)[i]->clone();
        }
    }
}

// ---- setShipsArr ----
// Station::setShips(Array<Ship*>*) — the single-argument overload used by save
// loading: drop any existing ships array and adopt the freshly-built one as-is
// (the elements are already owned by us, so no per-element clone like setShips()).
void Station::setShipsArr(void *ships) {
    Array<Ship*> *src = (Array<Ship*> *)ships;
    if (this->ships != src) {
        if (this->ships != 0) {
            for (Ship *s : *this->ships) delete s;
            this->ships->clear();
            if (this->ships != 0)
                delete this->ships;
        }
        this->ships = src;
    }
}

// ---- ArrayRemove<Ship*> ----
// Compact the ships array in place, removing every slot that equals `ship`.
void Station::arrayRemoveShip(Ship *ship, void *ships) {
    Array<Ship*> *arr = (Array<Ship*> *)ships;
    uint32_t n = arr->size();
    uint32_t kept = 0;
    for (uint32_t i = 0; i < n; i++) {
        Ship *cur = (*arr)[i];
        if (cur != ship)
            (*arr)[kept++] = cur;
    }
    arr->resize(kept);
}

// ---- baseDtor / dtorFinish ----
// Base-class teardown: the Station's first member is the engine String name, so the
// final step of ~Station destroys that String sub-object.
void Station::baseDtor() {
    ((String *)this)->dtor();
}

// Deleting-destructor tail: once ~Station has released the members, free the object.
void Station::dtorFinish() {
    ::operator delete(this);
}

// Global slot holding a pointer P; *P -> Q; *(Q+0x80) is the compared value.
extern int **const gAlienAttackSingleton __attribute__((visibility("hidden")));

bool Station::isAttackedByAliens() {
    return this->index == *(int *)((char *)(*gAlienAttackSingleton) + 0x80);
}

struct Ship;
// ArrayRemove<Ship*>(Ship* ship, Array* ships) — tail-called veneer.

// Station::removeShip(Ship*) — this in r0, ship in r1.
void Station::removeShip(Ship *ship) {
    void *ships = this->ships;
    if (ships == 0)
        return;
    Station::arrayRemoveShip(ship, ships);
}

uint32_t Station::getHiddenBlueprintIndex() {
    uint32_t i = 0;
    while (true) {
        if (i > 4)
            return 0xffffffff;
        if (kHiddenBlueprints[i] == this->index)
            return i;
        i++;
    }
}

struct Galaxy;

// Global slot -> P; *P -> the Galaxy* singleton.
extern Galaxy **const gGalaxySingleton __attribute__((visibility("hidden")));

uint8_t Station::isDiscovered() {
    char *visited = (char *)((Galaxy *)(*gGalaxySingleton))->getVisited();
    return visited[this->index];
}

void Station::setAttackedFriends(bool v) {
    this->attackedFriends = v;
}

uint8_t Station::hasAttackedFriends() {
    return this->attackedFriends;
}

uint32_t Station::getPirateStationIndex() {
    uint32_t i = 0;
    while (true) {
        if (i > 3)
            return 0xffffffff;
        if (kPirateStations[i] == this->index)
            return i;
        i++;
    }
}

// Station::addItem(Item*) — this in r0, item in r1.
void Station::addItem(Item *item) {
    Array<Item*> *arr = this->items;
    if (arr == 0) {
        arr = new Array<Item*>();
        this->items = arr;
    } else {
        uint32_t n = arr->size();
        if (n != 0) {
            for (uint32_t i = 0; i < n; i++) {
                if ((*arr)[i]->equals(item) != 0) {
                    Item *found = (*this->items)[i];
                    found->addAmount(item->getAmount());
                    return;
                }
                arr = this->items;
                n = arr->size();
            }
        }
    }
    arr->push_back(item);
}

// Station::clone() — this in r0, returns a new Station copy.
Station * Station::clone() {
    Station *n = (Station *)::operator new(0x34);
    char tmp[12];
    String_copy_ctor(tmp, this, false);
    ((Station *)(n))->ctor(tmp, this->index, this->systemIndex, this->techLevel, this->textureIndex);
    ((String *)(tmp))->dtor();
    return n;
}

// Station::~Station() — real C++ destructor so the demangled symbol contains "~Station".

// Status singleton: global slot holds P (kept in a reg); *P is reloaded per call.
extern Status **const gStatusForDtor __attribute__((visibility("hidden")));
#define STATUS (*gStatusForDtor)

// Station::~Station()
void Station::dtor() {
    if (this->ships != 0) {
        for (Ship *s : *this->ships) delete s;
        this->ships->clear();
        if (this->ships != 0)
            delete this->ships;
        this->ships = 0;
    }
    if (this->items != 0) {
        for (Item *it : *this->items) delete it;
        this->items->clear();
        if (this->items != 0)
            delete this->items;
        this->items = 0;
    }
    Array<Agent*> *agents = this->agents;
    if (agents != 0) {
        for (uint32_t i = 0; i < agents->size(); i++) {
            Agent *a = (*agents)[i];
            Agent *campA = ((Status *)(STATUS))->getCampaignMission() == 0
                               ? (Agent *)0
                               : ((Mission *)((Mission *)(intptr_t)((Status *)(STATUS))->getCampaignMission()))->getAgent();
            Agent *freeA = ((Status *)(STATUS))->getFreelanceMission() == 0
                               ? (Agent *)0
                               : ((Mission *)(((Status *)(STATUS))->getFreelanceMission()))->getAgent();
            if (a != 0 && a != campA && a != freeA && ((Agent *)(a))->isStoryAgent() == 0)
                ::operator delete(Agent_dtor(a));
            agents = this->agents;
        }
        if (agents != 0)
            delete agents;
        this->agents = 0;
    }
    ((Station *)(this))->baseDtor();
}

struct Agent;

void Station::setAgents(void *agents) {
    Array<Agent*> *src = (Array<Agent*> *)agents;
    if (this->agents != src) {
        if (this->agents != 0) {
            for (Agent *a : *this->agents) delete a;
            this->agents->clear();
            if (this->agents != 0)
                delete this->agents;
        }
        this->agents = src;
    }
}

extern "C" void *String_default_ctor(void *s);                 // String::String() -> this
extern "C" void String_from_cstr(void *out, const char *s, bool); // String::String(const char*, bool)
// operator=(String*, String*)

extern const char kStationDefaultName[] __attribute__((visibility("hidden")));

// Station::Station() — default ctor.
void Station::ctor_default() {
    String_default_ctor(this);
    char tmp[12];
    String_from_cstr(tmp, kStationDefaultName, false);
    ((String *)(this))->assign((String *)tmp);
    ((String *)(tmp))->dtor();
    this->index = -1;
    this->systemIndex = -1;
    this->techLevel = 0;
    this->textureIndex = 0;
    this->visited = 0;
    this->attackedFriends = 0;
    this->items = 0;
    this->ships = 0;
    this->agents = 0;
}

struct Ship;

// Station::hasShip(int) — this in r0, index in r1.
uint32_t Station::hasShip(int index) {
    Array<Ship*> *arr = this->ships;
    if (arr != 0) {
        for (uint32_t i = 0; i < arr->size(); i++) {
            Ship *sh = (*arr)[i];
            if (sh != 0) {
                if (sh->getIndex() == index)
                    return 1;
                arr = this->ships;
            }
        }
    }
    return 0;
}

struct Item;

// Station::hasItem(int) — this in r0, index in r1.
uint32_t Station::hasItem(int index) {
    Array<Item*> *arr = this->items;
    if (arr != 0) {
        for (uint32_t i = 0; i < arr->size(); i++) {
            Item *it = (*arr)[i];
            if (it != 0) {
                if (it->getIndex() == index)
                    return 1;
                arr = this->items;
            }
        }
    }
    return 0;
}

// Station::addShip(Ship*) — this in r0, ship in r1.
void Station::addShip(Ship *ship) {
    Array<Ship*> *arr = this->ships;
    if (arr == 0) {
        arr = new Array<Ship*>();
        this->ships = arr;
    } else {
        uint32_t n = arr->size();
        if (n != 0) {
            for (uint32_t i = 0; i < n; i++) {
                if ((*arr)[i]->equals(ship) != 0) {
                    return;
                }
                arr = this->ships;
                n = arr->size();
            }
        }
    }
    arr->push_back(ship);
}

extern "C" void *String_default_ctor(void *s);         // String::String() -> this
// operator=(String*, String*)

// Station::Station(String, int, int, int, int)
// r0=this, r1=String* param, r2=p3, r3=p4, [stack]=p5, p6.
Station * Station::ctor(void *name, int p3, int p4, int p5, int p6) {
    void *s = String_default_ctor(this);
    ((String *)(s))->assign((String *)name);
    this->index = p3;
    this->systemIndex = p4;
    this->techLevel = p5;
    this->textureIndex = p6;
    this->visited = 0;
    this->attackedFriends = 0;
    this->items = 0;
    this->ships = 0;
    this->agents = 0;
    return this;
}
