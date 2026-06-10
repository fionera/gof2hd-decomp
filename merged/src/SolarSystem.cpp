#include "SolarSystem.h"


extern "C" void ArrayRelease_int(void *a) __attribute__((nothrow));
extern "C" void *Array_int_dtor(void *a) __attribute__((nothrow));
extern "C" void SolarSystem_operator_delete(void *p) __attribute__((nothrow));
extern "C" void SolarSystem_baseStringDtor(void *strField) __attribute__((nothrow));
extern "C" Station *Status_getStation(Status *s);
extern "C" int Station_getIndex(Station *st);
extern "C" void String_copy_ctor(void *out, void *src, bool);
extern "C" int SolarSystem_stationIsInSystem_int(SolarSystem *self, int idx);
extern "C" int SolarSystem_warpGateLookup(SolarSystem *self, int idx);
extern "C" char *Galaxy_getVisited(Galaxy *g);

// ---- _SolarSystem_155694.cpp ----
// SolarSystem::~SolarSystem() — real C++ destructor so the demangled symbol contains "~SolarSystem".
// Releases the three Array<int> members at +0x38, +0x3c, +0x40, then the String at +0xc.

typedef unsigned int uint32_t;

// AbyssEngine::String::~String(String*) on the member at +0xc.

template <class T>
struct Array { uint32_t length; T *data; uint32_t cap; };

struct SolarSystem {
    unsigned char pad_0[0x38];
    Array<int> *stations;   // 0x38
    Array<int> *routes;     // 0x3c
    Array<int> *systems;    // 0x40
    char tail[0x4];         // 0x44 (visible flag etc.)
    ~SolarSystem() noexcept(false);
};

__attribute__((minsize)) SolarSystem::~SolarSystem() noexcept(false)
{
    if (this->stations != 0) {
        ArrayRelease_int(this->stations);
        if (this->stations != 0)
            SolarSystem_operator_delete(Array_int_dtor(this->stations));
    }
    this->stations = 0;
    if (this->routes != 0) {
        ArrayRelease_int(this->routes);
        if (this->routes != 0)
            SolarSystem_operator_delete(Array_int_dtor(this->routes));
    }
    this->routes = 0;
    if (this->systems != 0) {
        ArrayRelease_int(this->systems);
        if (this->systems != 0)
            SolarSystem_operator_delete(Array_int_dtor(this->systems));
    }
    this->systems = 0;
    SolarSystem_baseStringDtor((char *)this + 0xc);
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
extern "C" bool SolarSystem_currentOrbitHasWarpGate(SolarSystem *self)
{
    int orbit = F<int>(self, 0x30);
    Station *st = Status_getStation(*(Status **)gStatusOrbit);
    return orbit == Station_getIndex(st);
}

// ---- isVisible_1558d6.cpp ----
// SolarSystem::isVisible() — ldrb.w r0,[r0,#0x44]; bx lr
extern "C" uint8_t SolarSystem_isVisible(SolarSystem *self)
{
    return u8(self, 0x44);
}

// ---- stationIsInSystem_15576c.cpp ----
// SolarSystem::stationIsInSystem(int) — scan station-index array at +0x38.
extern "C" int SolarSystem_stationIsInSystem_int(SolarSystem *self, int idx)
{
    uint32_t *arr = F<uint32_t *>(self, 0x38);
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
extern "C" int SolarSystem_systemIsInSystemRoutes(SolarSystem *self, int sys)
{
    if (F<int>(self, 0x18) != sys) {
        uint32_t *arr = F<uint32_t *>(self, 0x40);
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
extern "C" void SolarSystem_setVisible(SolarSystem *self, bool v)
{
    u8(self, 0x44) = v;
}

// ---- getStationEnumIndex_1557f6.cpp ----
// SolarSystem::getStationEnumIndex(int) — index of matching station in array at +0x38, or -1.
extern "C" uint32_t SolarSystem_getStationEnumIndex(SolarSystem *self, int idx)
{
    uint32_t *arr = F<uint32_t *>(self, 0x38);
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
struct __attribute__((aligned(4))) RetStr { uint32_t a, b, c; };

extern "C" RetStr SolarSystem_getName(SolarSystem *self)
{
    RetStr r;
    String_copy_ctor(&r, (char *)self + 0xc, false);
    return r;
}

// ---- hasPirateBase_155720.cpp ----
// Pirate-base station-index table (PC-relative global).
extern const int kPirateBaseStations[4] __attribute__((visibility("hidden")));
// Singleton: global holds P; **P is the status/base struct pointer kept in r8.
extern void *gPirateBaseRoot __attribute__((visibility("hidden")));

// SolarSystem::hasPirateBase() — a pirate station in this system with its flag cleared.
extern "C" int SolarSystem_hasPirateBase(SolarSystem *self)
{
    char *base = *(char **)gPirateBaseRoot;
    uint32_t i = 0;
    while (true) {
        if (i > 3)
            return 0;
        if (SolarSystem_stationIsInSystem_int(self, kPirateBaseStations[i]) != 0) {
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

extern "C" int SolarSystem_getAttackRace(SolarSystem *self)
{
    uint32_t r = F<uint32_t>(self, 0x20);
    if (r < 4)
        return kAttackRaceTable[r];
    return 8;
}

// ---- hasNoOwner_1558e2.cpp ----
// SolarSystem::hasNoOwner() — owner race at +0x18; map a contiguous range to a bitmask.
// (race-0x17) < 0xb ? (0x60b >> (x & 0xff)) & 1 : 0
extern "C" uint32_t SolarSystem_hasNoOwner(SolarSystem *self)
{
    uint32_t x = F<uint32_t>(self, 0x18) - 0x17;
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
extern "C" int SolarSystem_hasHiddenBlueprint(SolarSystem *self)
{
    char *base = *(char **)gBlueprintRoot;
    uint32_t i = 0;
    while (true) {
        if (i > 4)
            return 0;
        if (SolarSystem_stationIsInSystem_int(self, kBlueprintStations[i]) != 0) {
            char *flags = *(char **)(*(char **)(base + 0x58) + 4);
            if (flags[i] == 0)
                return 1;
        }
        i++;
    }
}

// ---- setCoords_15589c.cpp ----
// SolarSystem::setCoords(int, int) — strd r1,r2,[r0,#0x24]; bx lr
extern "C" void SolarSystem_setCoords(SolarSystem *self, int x, int y)
{
    F<int>(self, 0x24) = x;
    F<int>(self, 0x28) = y;
}

// ---- SolarSystem_1555dc.cpp ----
// AbyssEngine::String operations.
extern "C" void *String_default_ctor(void *self);                 // 0x6efbc -> this
extern "C" void String_copy_ctor(void *out, const void *src, bool); // 0x6f028
extern "C" void String_assign(void *dst, const void *rhs);        // 0x6f2b0
extern "C" void String_dtor(void *self);                          // 0x6ee30

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
extern "C" SolarSystem *
SolarSystem_ctor(SolarSystem *self, int p1, const String12 &p2, int p3, bool p4,
                 int p5, int p6, int p7, int p8, int p9, int p10,
                 int *p11, void *p12, void *p13, void *p14)
{
    String_default_ctor((char *)self + 0xc);
    F<int>(self, 0x18) = p1;
    char tmp[12];
    String_copy_ctor(tmp, &p2, false);
    String_assign((char *)self + 0xc, tmp);
    String_dtor(tmp);
    u8(self, 0x44) = p4;
    F<int>(self, 0x1c) = p3;
    F<int>(self, 0x20) = p5;
    F<int>(self, 0x24) = p6;
    F<int>(self, 0x28) = p7;
    F<int>(self, 0x2c) = p8;
    F<int>(self, 0x30) = p9;
    F<int>(self, 0x34) = p10;
    F<int>(self, 0x00) = p11[0];
    F<int>(self, 0x04) = p11[1];
    int v2 = p11[2];
    F<void *>(self, 0x38) = p12;
    F<int>(self, 0x08) = v2;
    F<void *>(self, 0x3c) = p14;
    F<void *>(self, 0x40) = p13;
    return self;
}

// ---- getWarpGateEnumIndex_1557f0.cpp ----
// SolarSystem::getWarpGateEnumIndex() — ldr r1,[r0,#0x30]; b.w <veneer>
// Tail-calls a helper with (this, *(this+0x30)).
// Byte-exact: the function body is exactly these 2 instructions (6 bytes). methods.tsv lists
// nbytes=8, so the diff window over-reads 2 bytes into the adjacent function and reports a
// spurious 3rd instruction; checked with the true 6-byte window it is `>>> EXACT MATCH`.

extern "C" int SolarSystem_getWarpGateEnumIndex(SolarSystem *self)
{
    return SolarSystem_warpGateLookup(self, F<int>(self, 0x30));
}

// ---- isFullyDiscovered_15585c.cpp ----
struct Galaxy;
// Galaxy::getVisited() -> char* visited-flags table.

// Galaxy singleton: global variable holds a Galaxy*; one register caches that pointer,
// and the Galaxy's first member is passed to getVisited each iteration.
extern Galaxy *gGalaxyDiscover __attribute__((visibility("hidden")));

// SolarSystem::isFullyDiscovered() — every station in array +0x38 must be visited.
extern "C" int SolarSystem_isFullyDiscovered(SolarSystem *self)
{
    uint32_t *arr = F<uint32_t *>(self, 0x38);
    uint32_t i = 0;
    Galaxy *gal = gGalaxyDiscover;
    while (true) {
        if (i >= arr[0])
            return 1;
        char *visited = Galaxy_getVisited(*(Galaxy **)gal);
        arr = F<uint32_t *>(self, 0x38);
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
extern "C" int SolarSystem_stationIsInSystem(SolarSystem *self, Station *st)
{
    if (st != 0)
        return SolarSystem_stationIsInSystem_int(self, Station_getIndex(st));
    return 0;
}
