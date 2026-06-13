#include "gof2/game/world/SolarSystem.h"
#include "gof2/game/world/Galaxy.h"
#include "gof2/game/mission/Status.h"
#include "gof2/game/core/String.h"

extern "C" int Station_getIndex(Station *st);

// SolarSystem::~SolarSystem() — real C++ destructor so the demangled symbol contains "~SolarSystem".
// Releases the three Array<int> members at +0x38, +0x3c, +0x40, then the String at +0xc.

using AbyssEngine::String;

// AbyssEngine::String::~String(String*) on the member at +0xc.
// SolarSystem::~SolarSystem() — releases the three Array<int> members at +0x38,
// +0x3c, +0x40, then the String at +0xc.

void SolarSystem::dtor() {
    if (this->stationIds != 0) {
        this->stationIds->clear();
        delete this->stationIds;
    }
    this->stationIds = 0;
    if (this->field_0x3c != 0) {
        this->field_0x3c->clear();
        delete this->field_0x3c;
    }
    this->field_0x3c = 0;
    if (this->linkedSystemIds != 0) {
        this->linkedSystemIds->clear();
        delete this->linkedSystemIds;
    }
    this->linkedSystemIds = 0;
    SolarSystem::baseStringDtor(&this->name);
}

struct Status;
struct Station;
// Status::getStation() and Station::getIndex(Station*) — resolved blx targets.

// Singleton slot: global variable holds a Status*; the Status's first member is
// passed to getStation. Single-level pointer so the access is:
//   ldr r1,[lit]; add r1,pc; ldr r1,[r1] (= the Status*); ldr r0,[r1] (= field 0)
extern Status *gStatusOrbit __attribute__((visibility("hidden")));

// SolarSystem::currentOrbitHasWarpGate() — orbit field at +0x30 vs current station index.
bool SolarSystem::currentOrbitHasWarpGate() {
    int orbit = this->jumpgateStationId;
    Station *st = ((Status *)(*(Status **)gStatusOrbit))->getStation();
    return orbit == Station_getIndex(st);
}

// ---- simple field accessors (1656ec..1658aa) ----
// Plain getters; each is a single load from the corresponding member.
int SolarSystem::getIndex()          { return systemId; }            // +0x18
int SolarSystem::getRace()           { return (int)faction; }        // +0x20
int SolarSystem::getSecurityLevel()  { return securityLevel; }       // +0x1c
int SolarSystem::getTextureIndex()   { return textureIndex; }        // +0x34
int SolarSystem::getX()              { return mapX; }                // +0x24
int SolarSystem::getY()              { return mapY; }                // +0x28
int SolarSystem::getZ()              { return mapZ; }                // +0x2c
int SolarSystem::getWarpGateIndex()  { return jumpgateStationId; }   // +0x30
uint32_t *SolarSystem::getStations() { return (uint32_t *)stationIds; }      // +0x38
uint32_t *SolarSystem::getRoutes()   { return (uint32_t *)linkedSystemIds; } // +0x40

// Array<int>*-typed view of the station-id list — the very same member at +0x38
// that getStations() returns. Status::activateNewWanted() uses it to pick a
// random station in a destination system (it then reads the embedded data
// pointer at +4 and indexes into it).
uint32_t *SolarSystem::getStations_i() { return (uint32_t *)stationIds; }     // +0x38

// SolarSystem::warpGateLookup(int) — getWarpGateEnumIndex() forwards here with
// the orbit's jump-gate station id. Resolving the warp-gate enum index is the
// same array lookup used for ordinary stations, so reuse getStationEnumIndex().
int SolarSystem::warpGateLookup(int idx) {
    return (int)getStationEnumIndex(idx);
}

// SolarSystem::baseStringDtor() — runs AbyssEngine::String::~String() on the
// in-place `name` member (passed by address from ~SolarSystem()). This is the
// base (non-deleting) destructor: it releases the string's backing buffer but
// does not free the SolarSystem itself.
void SolarSystem::baseStringDtor(void *strField) {
    ((String *)strField)->dtor();
}

// SolarSystem::isVisible() — ldrb.w r0,[r0,#0x44]; bx lr
uint8_t SolarSystem::isVisible() {
    return this->visible;
}

// SolarSystem::stationIsInSystem(int) — scan station-index array at +0x38.
int SolarSystem::stationIsInSystem_int(int idx) {
    Array<int> *arr = this->stationIds;
    uint32_t n = arr->size();
    for (uint32_t i = 0; i < n; i++) {
        if ((*arr)[i] == idx)
            return 1;
    }
    return 0;
}

// SolarSystem::systemIsInSystemRoutes(int) — self's own system (+0x18) counts; otherwise
// scan the routes array at +0x40 (null -> not present).
int SolarSystem::systemIsInSystemRoutes(int sys) {
    if (this->systemId != sys) {
        Array<int> *arr = this->linkedSystemIds;
        if (arr == 0)
            return 0;
        uint32_t n = arr->size();
        for (uint32_t i = 0; i < n; i++) {
            if ((*arr)[i] == sys)
                return 1;
        }
        return 0;
    }
    return 1;
}

// SolarSystem::setVisible(bool) — strb.w r1,[r0,#0x44]; bx lr
void SolarSystem::setVisible(bool v) {
    this->visible = v;
}

// SolarSystem::getStationEnumIndex(int) — index of matching station in array at +0x38, or -1.
uint32_t SolarSystem::getStationEnumIndex(int idx) {
    Array<int> *arr = this->stationIds;
    for (uint32_t i = 0; i < arr->size(); i++) {
        if ((*arr)[i] == idx)
            return i;
    }
    return 0xffffffff;
}

// AbyssEngine::String::String(String* out, const String* src, bool) -> void (0x6f028)

// SolarSystem::getName() -> String by value (sret in r0, this in r1).
// Copies the String member at +0xc. The copy-ctor returns void, so the compiler
// keeps a frame and restores the sret pointer.
// String is defined in gof2/SolarSystem.h.

String SolarSystem::getName() {
    return this->name;
}

// Pirate-base station-index table (PC-relative global).
extern const int kPirateBaseStations[4] __attribute__((visibility("hidden")));
// Singleton: global holds P; **P is the status/base struct pointer kept in r8.
extern void *gPirateBaseRoot __attribute__((visibility("hidden")));

// SolarSystem::hasPirateBase() — a pirate station in this system with its flag cleared.
int SolarSystem::hasPirateBase() {
    char *base = *(char **)gPirateBaseRoot;
    uint32_t i = 0;
    while (true) {
        if (i > 3)
            return 0;
        if (((SolarSystem *)(this))->stationIsInSystem_int(kPirateBaseStations[i]) != 0) {
            char *flags = *(char **)(*(char **)(base + 0x4c) + 4);
            if (flags[i] == 0)
                return 1;
        }
        i++;
    }
}

// SolarSystem::getAttackRace() — field at 0x20 indexes a 4-entry table; >=4 -> 8.
// The table is a PC-relative global (visibility hidden -> add r1,pc form).
extern const int kAttackRaceTable[4] __attribute__((visibility("hidden")));

int SolarSystem::getAttackRace() {
    uint32_t r = this->faction;
    if (r < 4)
        return kAttackRaceTable[r];
    return 8;
}

// SolarSystem::hasNoOwner() — owner race at +0x18; map a contiguous range to a bitmask.
// (race-0x17) < 0xb ? (0x60b >> (x & 0xff)) & 1 : 0
uint32_t SolarSystem::hasNoOwner() {
    uint32_t x = this->systemId - 0x17;
    if (x < 0xb)
        return (0x60bU >> (x & 0x7ff)) & 1;
    return 0;
}

// Hidden-blueprint station-index table (PC-relative global).
extern const int kBlueprintStations[5] __attribute__((visibility("hidden")));
// Singleton: global holds P; **P is the status/base struct pointer kept in r8.
extern void *gBlueprintRoot __attribute__((visibility("hidden")));

// SolarSystem::hasHiddenBlueprint() — a blueprint station in this system with its flag cleared.
int SolarSystem::hasHiddenBlueprint() {
    char *base = *(char **)gBlueprintRoot;
    uint32_t i = 0;
    while (true) {
        if (i > 4)
            return 0;
        if (((SolarSystem *)(this))->stationIsInSystem_int(kBlueprintStations[i]) != 0) {
            char *flags = *(char **)(*(char **)(base + 0x58) + 4);
            if (flags[i] == 0)
                return 1;
        }
        i++;
    }
}

// SolarSystem::setCoords(int, int) — strd r1,r2,[r0,#0x24]; bx lr
void SolarSystem::setCoords(int x, int y) {
    this->mapX = x;
    this->mapY = y;
}

// AbyssEngine::String operations.
// SolarSystem::SolarSystem(int, String, int, bool, int*6, int*, Array*, Array*, Array*)
// The String arg has a non-trivial copy ctor/dtor, so the ABI passes it by invisible
// reference (a pointer in r2). The on-stack `name` build uses a stack-protector canary.
//
// NOTE: near-match only. The field assignments and String build are semantically
// correct and the canary is emitted (char[] temp), but byte-exact is resistant:
// clang -Oz colors the param/this+0xc registers differently and coalesces the
// consecutive stacked-arg loads/field stores into ldmia/stmia + strd pairs, whereas
// the target keeps them as discrete ldr/str. This is dominated by register-allocation
// and frame-layout choices not reachable from source form.
SolarSystem * SolarSystem::ctor(int p1, const String &p2, int p3, bool p4, int p5, int p6, int p7, int p8, int p9, int p10, int *p11, void *p12, void *p13, void *p14) {
    this->name.ctor();
    this->systemId = p1;
    String tmp;
    tmp.ctor_copy((String *)(&p2), false);
    this->name.assign(&tmp);
    this->visible = p4;
    this->securityLevel = p3;
    this->faction = p5;
    this->mapX = p6;
    this->mapY = p7;
    this->mapZ = p8;
    this->jumpgateStationId = p9;
    this->textureIndex = p10;
    this->field_0x0 = p11[0];
    this->field_0x4 = p11[1];
    int v2 = p11[2];
    this->stationIds = (Array<int> *)p12;
    this->field_0x8 = v2;
    this->field_0x3c = (Array<int> *)p14;
    this->linkedSystemIds = (Array<int> *)p13;
    return this;
}

// SolarSystem::getWarpGateEnumIndex() — ldr r1,[r0,#0x30]; b.w <veneer>
// Tail-calls a helper with (this, *(this+0x30)).
// Byte-exact: the function body is exactly these 2 instructions (6 bytes). methods.tsv lists
// nbytes=8, so the diff window over-reads 2 bytes into the adjacent function and reports a
// spurious 3rd instruction; checked with the true 6-byte window it is `>>> EXACT MATCH`.

int SolarSystem::getWarpGateEnumIndex() {
    return ((SolarSystem *)(this))->warpGateLookup(this->jumpgateStationId);
}

struct Galaxy;
// Galaxy::getVisited() -> char* visited-flags table.

// Galaxy singleton: global variable holds a Galaxy*; one register caches that pointer,
// and the Galaxy's first member is passed to getVisited each iteration.
extern Galaxy *gGalaxyDiscover __attribute__((visibility("hidden")));

// SolarSystem::isFullyDiscovered() — every station in array +0x38 must be visited.
int SolarSystem::isFullyDiscovered() {
    Array<int> *arr = this->stationIds;
    uint32_t i = 0;
    Galaxy *gal = gGalaxyDiscover;
    while (true) {
        if (i >= arr->size())
            return 1;
        char *visited = (char *)((Galaxy *)(*(Galaxy **)gal))->getVisited();
        arr = this->stationIds;
        uint32_t flagIdx = (*arr)[i];
        i++;
        if (visited[flagIdx] == 0)
            return 0;
    }
}

struct Station;
// SolarSystem::stationIsInSystem(int) overload, tail-called.

// SolarSystem::stationIsInSystem(Station*) — null -> 0, else dispatch by station index.
// NOTE: near-match only. The body (getIndex + tail-call to the int overload) is
// byte-identical, but clang -Oz schedules the trivial `st==0 -> return 0` path
// before the prologue (cbz; ...; bx lr), whereas the target saves `this` in r4
// first and shares the pop epilogue. Resistant to source-level restructuring.
int SolarSystem::stationIsInSystem(Station *st) {
    if (st != 0)
        return ((SolarSystem *)(this))->stationIsInSystem_int(Station_getIndex(st));
    return 0;
}
