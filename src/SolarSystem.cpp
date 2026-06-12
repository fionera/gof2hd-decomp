#include "gof2/SolarSystem.h"
#include "gof2/Galaxy.h"
#include "gof2/Status.h"
#include "gof2/String.h"


extern "C" void ArrayRelease_int(void *a) __attribute__((nothrow));
extern "C" void *Array_int_dtor(void *a) __attribute__((nothrow));
extern "C" void SolarSystem_baseStringDtor(void *strField) __attribute__((nothrow));
extern "C" int Station_getIndex(Station *st);
extern "C" int SolarSystem_warpGateLookup(SolarSystem *self, int idx);

// ---- _SolarSystem_155694.cpp ----
// SolarSystem::~SolarSystem() — real C++ destructor so the demangled symbol contains "~SolarSystem".
// Releases the three Array<int> members at +0x38, +0x3c, +0x40, then the String at +0xc.

using AbyssEngine::String12;

// AbyssEngine::String::~String(String*) on the member at +0xc.
// SolarSystem::~SolarSystem() — releases the three Array<int> members at +0x38,
// +0x3c, +0x40, then the String at +0xc.

void SolarSystem::dtor() {
    SolarSystem *self = this;
    if (self->stationIds != 0) {
        ArrayRelease_int(self->stationIds);
        if (self->stationIds != 0)
            ::operator delete(Array_int_dtor(self->stationIds));
    }
    self->stationIds = 0;
    if (self->field_0x3c != 0) {
        ArrayRelease_int(self->field_0x3c);
        if (self->field_0x3c != 0)
            ::operator delete(Array_int_dtor(self->field_0x3c));
    }
    self->field_0x3c = 0;
    if (self->linkedSystemIds != 0) {
        ArrayRelease_int(self->linkedSystemIds);
        if (self->linkedSystemIds != 0)
            ::operator delete(Array_int_dtor(self->linkedSystemIds));
    }
    self->linkedSystemIds = 0;
    SolarSystem_baseStringDtor((char *)self + 0xc);
}

// ---- currentOrbitHasWarpGate_155818.cpp ----
struct Status;
struct Station;
// Status::getStation() and Station::getIndex(Station*) — resolved blx targets.

// Singleton slot: global variable holds a Status*; the Status's first member is
// passed to getStation. Single-level pointer so the access is:
//   ldr r1,[lit]; add r1,pc; ldr r1,[r1] (= the Status*); ldr r0,[r1] (= field 0)
extern Status *gStatusOrbit __attribute__((visibility("hidden")));

// SolarSystem::currentOrbitHasWarpGate() — orbit field at +0x30 vs current station index.
bool SolarSystem::currentOrbitHasWarpGate() {
    SolarSystem *self = this;
    int orbit = self->jumpgateStationId;
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
uint32_t *SolarSystem::getStations() { return stationIds; }          // +0x38
uint32_t *SolarSystem::getRoutes()   { return linkedSystemIds; }     // +0x40

// ---- isVisible_1558d6.cpp ----
// SolarSystem::isVisible() — ldrb.w r0,[r0,#0x44]; bx lr
uint8_t SolarSystem::isVisible() {
    SolarSystem *self = this;
    return u8(self, 0x44);
}

// ---- stationIsInSystem_15576c.cpp ----
// SolarSystem::stationIsInSystem(int) — scan station-index array at +0x38.
int SolarSystem::stationIsInSystem_int(int idx) {
    SolarSystem *self = this;
    uint32_t *arr = self->stationIds;
    uint32_t n = arr[0];
    for (uint32_t i = 0; i < n; i++) {
        if (((int *)arr[1])[i] == idx)
            return 1;
    }
    return 0;
}

// ---- systemIsInSystemRoutes_1558ae.cpp ----
// SolarSystem::systemIsInSystemRoutes(int) — self's own system (+0x18) counts; otherwise
// scan the routes array at +0x40 (null -> not present).
int SolarSystem::systemIsInSystemRoutes(int sys) {
    SolarSystem *self = this;
    if (self->systemId != sys) {
        uint32_t *arr = self->linkedSystemIds;
        if (arr == 0)
            return 0;
        uint32_t n = arr[0];
        for (uint32_t i = 0; i < n; i++) {
            if (((int *)arr[1])[i] == sys)
                return 1;
        }
        return 0;
    }
    return 1;
}

// ---- setVisible_1558dc.cpp ----
// SolarSystem::setVisible(bool) — strb.w r1,[r0,#0x44]; bx lr
void SolarSystem::setVisible(bool v) {
    SolarSystem *self = this;
    u8(self, 0x44) = v;
}

// ---- getStationEnumIndex_1557f6.cpp ----
// SolarSystem::getStationEnumIndex(int) — index of matching station in array at +0x38, or -1.
uint32_t SolarSystem::getStationEnumIndex(int idx) {
    SolarSystem *self = this;
    uint32_t *arr = self->stationIds;
    for (uint32_t i = 0; i < arr[0]; i++) {
        if (((int *)arr[1])[i] == idx)
            return i;
    }
    return 0xffffffff;
}

// ---- getName_1556f0.cpp ----
// AbyssEngine::String::String(String* out, const String* src, bool) -> void (0x6f028)

// SolarSystem::getName() -> String by value (sret in r0, this in r1).
// Copies the String member at +0xc. The copy-ctor returns void, so the compiler
// keeps a frame and restores the sret pointer.
// RetStr is defined in gof2/SolarSystem.h.

RetStr SolarSystem::getName() {
    SolarSystem *self = this;
    RetStr r;
    ((String *)(&r))->ctor_copy((String *)((char *)self + 0xc), false);
    return r;
}

// ---- hasPirateBase_155720.cpp ----
// Pirate-base station-index table (PC-relative global).
extern const int kPirateBaseStations[4] __attribute__((visibility("hidden")));
// Singleton: global holds P; **P is the status/base struct pointer kept in r8.
extern void *gPirateBaseRoot __attribute__((visibility("hidden")));

// SolarSystem::hasPirateBase() — a pirate station in this system with its flag cleared.
int SolarSystem::hasPirateBase() {
    SolarSystem *self = this;
    char *base = *(char **)gPirateBaseRoot;
    uint32_t i = 0;
    while (true) {
        if (i > 3)
            return 0;
        if (((SolarSystem *)(self))->stationIsInSystem_int(kPirateBaseStations[i]) != 0) {
            char *flags = *(char **)(*(char **)(base + 0x4c) + 4);
            if (flags[i] == 0)
                return 1;
        }
        i++;
    }
}

// ---- getAttackRace_155708.cpp ----
// SolarSystem::getAttackRace() — field at 0x20 indexes a 4-entry table; >=4 -> 8.
// The table is a PC-relative global (visibility hidden -> add r1,pc form).
extern const int kAttackRaceTable[4] __attribute__((visibility("hidden")));

int SolarSystem::getAttackRace() {
    SolarSystem *self = this;
    uint32_t r = self->faction;
    if (r < 4)
        return kAttackRaceTable[r];
    return 8;
}

// ---- hasNoOwner_1558e2.cpp ----
// SolarSystem::hasNoOwner() — owner race at +0x18; map a contiguous range to a bitmask.
// (race-0x17) < 0xb ? (0x60b >> (x & 0xff)) & 1 : 0
uint32_t SolarSystem::hasNoOwner() {
    SolarSystem *self = this;
    uint32_t x = self->systemId - 0x17;
    if (x < 0xb)
        return (0x60bU >> (x & 0x7ff)) & 1;
    return 0;
}

// ---- hasHiddenBlueprint_155790.cpp ----
// Hidden-blueprint station-index table (PC-relative global).
extern const int kBlueprintStations[5] __attribute__((visibility("hidden")));
// Singleton: global holds P; **P is the status/base struct pointer kept in r8.
extern void *gBlueprintRoot __attribute__((visibility("hidden")));

// SolarSystem::hasHiddenBlueprint() — a blueprint station in this system with its flag cleared.
int SolarSystem::hasHiddenBlueprint() {
    SolarSystem *self = this;
    char *base = *(char **)gBlueprintRoot;
    uint32_t i = 0;
    while (true) {
        if (i > 4)
            return 0;
        if (((SolarSystem *)(self))->stationIsInSystem_int(kBlueprintStations[i]) != 0) {
            char *flags = *(char **)(*(char **)(base + 0x58) + 4);
            if (flags[i] == 0)
                return 1;
        }
        i++;
    }
}

// ---- setCoords_15589c.cpp ----
// SolarSystem::setCoords(int, int) — strd r1,r2,[r0,#0x24]; bx lr
void SolarSystem::setCoords(int x, int y) {
    SolarSystem *self = this;
    self->mapX = x;
    self->mapY = y;
}

// ---- SolarSystem_1555dc.cpp ----
// AbyssEngine::String operations.
extern "C" void *String_default_ctor(void *self);                 // 0x6efbc -> this
// 0x6f2b0
// 0x6ee30

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
SolarSystem * SolarSystem::ctor(int p1, const String12 &p2, int p3, bool p4, int p5, int p6, int p7, int p8, int p9, int p10, int *p11, void *p12, void *p13, void *p14) {
    SolarSystem *self = this;
    String_default_ctor((char *)self + 0xc);
    self->systemId = p1;
    char tmp[12];
    ((String *)(tmp))->ctor_copy((String *)(&p2), false);
    ((String *)((char *)self + 0xc))->assign((String *)tmp);
    ((String *)(tmp))->dtor();
    u8(self, 0x44) = p4;
    self->securityLevel = p3;
    self->faction = p5;
    self->mapX = p6;
    self->mapY = p7;
    self->mapZ = p8;
    self->jumpgateStationId = p9;
    self->textureIndex = p10;
    self->field_0x0 = p11[0];
    self->field_0x4 = p11[1];
    int v2 = p11[2];
    self->stationIds = (uint32_t *)p12;
    self->field_0x8 = v2;
    self->field_0x3c = p14;
    self->linkedSystemIds = (uint32_t *)p13;
    return self;
}

// ---- getWarpGateEnumIndex_1557f0.cpp ----
// SolarSystem::getWarpGateEnumIndex() — ldr r1,[r0,#0x30]; b.w <veneer>
// Tail-calls a helper with (this, *(this+0x30)).
// Byte-exact: the function body is exactly these 2 instructions (6 bytes). methods.tsv lists
// nbytes=8, so the diff window over-reads 2 bytes into the adjacent function and reports a
// spurious 3rd instruction; checked with the true 6-byte window it is `>>> EXACT MATCH`.

int SolarSystem::getWarpGateEnumIndex() {
    SolarSystem *self = this;
    return SolarSystem_warpGateLookup(self, self->jumpgateStationId);
}

// ---- isFullyDiscovered_15585c.cpp ----
struct Galaxy;
// Galaxy::getVisited() -> char* visited-flags table.

// Galaxy singleton: global variable holds a Galaxy*; one register caches that pointer,
// and the Galaxy's first member is passed to getVisited each iteration.
extern Galaxy *gGalaxyDiscover __attribute__((visibility("hidden")));

// SolarSystem::isFullyDiscovered() — every station in array +0x38 must be visited.
int SolarSystem::isFullyDiscovered() {
    SolarSystem *self = this;
    uint32_t *arr = self->stationIds;
    uint32_t i = 0;
    Galaxy *gal = gGalaxyDiscover;
    while (true) {
        if (i >= arr[0])
            return 1;
        char *visited = (char *)((Galaxy *)(*(Galaxy **)gal))->getVisited();
        arr = self->stationIds;
        uint32_t flagIdx = ((int *)arr[1])[i];
        i++;
        if (visited[flagIdx] == 0)
            return 0;
    }
}

// ---- stationIsInSystem_15583c.cpp ----
struct Station;
// SolarSystem::stationIsInSystem(int) overload, tail-called.

// SolarSystem::stationIsInSystem(Station*) — null -> 0, else dispatch by station index.
// NOTE: near-match only. The body (getIndex + tail-call to the int overload) is
// byte-identical, but clang -Oz schedules the trivial `st==0 -> return 0` path
// before the prologue (cbz; ...; bx lr), whereas the target saves `this` in r4
// first and shares the pop epilogue. Resistant to source-level restructuring.
int SolarSystem::stationIsInSystem(Station *st) {
    SolarSystem *self = this;
    if (st != 0)
        return ((SolarSystem *)(self))->stationIsInSystem_int(Station_getIndex(st));
    return 0;
}
