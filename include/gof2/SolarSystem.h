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
using AbyssEngine::String12;

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
    uint32_t* stationIds;               // +0x38
    void* field_0x3c;                   // +0x3c
    uint32_t* linkedSystemIds;               // +0x40

    // ---- methods (converted from free functions) ----
    SolarSystem * ctor(int p1, const String12 &p2, int p3, bool p4, int p5, int p6, int p7, int p8, int p9, int p10, int *p11, void *p12, void *p13, void *p14);
    bool currentOrbitHasWarpGate();
    void dtor();
    int getAttackRace();
    RetStr getName();
    uint32_t getStationEnumIndex(int idx);
    int getWarpGateEnumIndex();
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
