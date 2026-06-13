#ifndef GOF2_WANTED_H
#define GOF2_WANTED_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — Wanted class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: namespace). Field offsets recovered from the
// per-method target disassembly. We do NOT model a full layout: methods access
// fields via byte-offset casts from `this` (which arrives in r0 just like the
// first explicit arg of an extern "C" function).

struct Wanted;

// AbyssEngine::String passed by value is a 12-byte trivially-copied aggregate
// (text*, size, ...). Model it opaquely so it is passed the same way as the
// target ABI (struct > 8 bytes by value -> on the stack / sret-style).

// The recovered code uses the bare name String for the 12-byte stack String
// temp. It lives in namespace AbyssEngine (see common.h); pull it into scope.
using AbyssEngine::String;

// 12-byte by-value String return aggregate (sret on the target ABI): getName()
// returns a 12-byte String temp. Modeled the same way as in Agent.h/Mission.h.

// Field accessor via byte offset.

class Wanted {
public:
    String name;                   // +0x0  (name, AbyssEngine::String)
    int index;                      // +0xc
    int board;                     // +0x10
    int race;                     // +0x14
    uint8_t male;                 // +0x18
    int shipId;                     // +0x1c
    int weapon;                     // +0x20
    int hitpoints;                     // +0x24
    int lootItemId;                     // +0x28
    int lootAmount;                     // +0x2c
    int reward;                     // +0x30
    int requiredBounties;                     // +0x34
    int requiredMission;                     // +0x38
    int numWingmen;                     // +0x3c
    void* imageParts;                   // +0x40  (heap buffer freed in dtor)
    int currentLocation;                     // +0x44
    int travelsTo;                     // +0x48
    int lastSeen;                     // +0x4c
    uint8_t terminated;                 // +0x50
    uint8_t active;                 // +0x51

    // ---- methods (converted from free functions) ----
    Wanted * ctor(int p1, const String &p2, int p3, int p4, bool p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13, int p14);
    Wanted * dtor();
    Wanted * base_dtor();   // base subobject dtor: destroys `name`, returns this
    String getName();
    uint8_t isActive();
    uint8_t isTerminated();
    void setActive(bool v);
    void setTerminated(bool v);

    int  getIndex();
    int  getBoard();
    int  getRace();
    int  isMale();
    int  getShip();
    int  getWeapon();
    int  getHitpoints();
    int  getLoot();
    int  getLootAmount();
    int  getReward();
    int  getRequiredBounties();
    int  getRequiredMission();
    int  getNumWingmen();
    int  getCurrentLocation();
    int  getTravelsTo();
    int  getLastSeen();
    int *getImageParts();
    void setImageParts(int *parts);
    void setCurrentLocation(int v);
    void setTravelsTo(int v);
    void setLastSeen(int v);
};
#endif
