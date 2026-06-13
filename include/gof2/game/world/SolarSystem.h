#ifndef GOF2_SOLARSYSTEM_H
#define GOF2_SOLARSYSTEM_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — SolarSystem class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: on the class itself), though it embeds an
// AbyssEngine::String at +0x0c. Field offsets recovered per-method from the
// target disassembly; methods access fields via byte-offset casts from `this`.

struct SolarSystem;

// Forward declaration: Station is only used through a pointer below.
struct Station;

// Bring the small stack-string temp into scope (declared in common.h).
using AbyssEngine::String;

// AbyssEngine::String passed/returned by value is a 12-byte trivially-copied
// aggregate (text*, size, ...). Model it opaquely so the ABI matches the target.

// Field accessors via byte offset from `this`.

class SolarSystem {
public:
    int field_0x0;                      // +0x0
    int field_0x4;                      // +0x4
    int field_0x8;                      // +0x8
    int systemId;                     // +0x18
    int securityLevel;                     // +0x1c
    uint32_t faction;                // +0x20
    int mapX;                     // +0x24
    int mapY;                     // +0x28
    int mapZ;                     // +0x2c
    int jumpgateStationId;                     // +0x30
    int textureIndex;                     // +0x34
    Array<int>* stationIds;             // +0x38
    Array<int>* field_0x3c;             // +0x3c
    Array<int>* linkedSystemIds;        // +0x40

    // ---- methods (converted from free functions) ----
    SolarSystem * ctor(int p1, const String &p2, int p3, bool p4, int p5, int p6, int p7, int p8, int p9, int p10, int *p11, void *p12, void *p13, void *p14);
    bool currentOrbitHasWarpGate();
    void dtor();
    int getAttackRace();
    int getIndex();
    int getRace();
    int getSecurityLevel();
    int getTextureIndex();
    int getX();
    int getY();
    int getZ();
    int getWarpGateIndex();
    uint32_t *getStations();
    // Array<int>*-typed view of the station-id list (same +0x38 member as
    // getStations()); callers read the embedded data pointer at +4.
    uint32_t *getStations_i();
    uint32_t *getRoutes();
    String getName();
    uint32_t getStationEnumIndex(int idx);
    int getWarpGateEnumIndex();
    // Resolve the warp-gate's enum index — the same table lookup as an ordinary
    // station, so it forwards to getStationEnumIndex().
    int warpGateLookup(int idx);
    // Run AbyssEngine::String::~String() on the in-place `name` member (the base,
    // non-deleting string destructor invoked from ~SolarSystem()).
    static void baseStringDtor(void *strField);
    int hasHiddenBlueprint();
    uint32_t hasNoOwner();
    int hasPirateBase();
    int isFullyDiscovered();
    uint8_t isVisible();
    void setCoords(int x, int y);
    void setVisible(bool v);
    int stationIsInSystem(Station *st);
    int stationIsInSystem_int(int idx);
    int systemIsInSystemRoutes(int sys);
};
#endif
