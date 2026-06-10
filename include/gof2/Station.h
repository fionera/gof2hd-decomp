#ifndef GOF2_STATION_H
#define GOF2_STATION_H
#include "gof2/common.h"
#include "gof2/Agent.h"   // provides the shared RetStr aggregate
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — Station class. Top-level, no namespace.
// Field offsets recovered from per-method target disassembly. Methods access
// fields via byte-offset casts from `this`.


struct Station;

// RetStr (AbyssEngine::String by value: 12-byte trivially-copied aggregate)
// is provided by gof2/Agent.h (included above) to avoid a duplicate definition.

struct Station {
    int field_0xc;                      // +0xc
    int field_0x10;                     // +0x10
    int field_0x18;                     // +0x18
    uint8_t field_0x1c;                 // +0x1c
    int field_0x20;                     // +0x20
    uint8_t field_0x24;                 // +0x24
    void* field_0x28;                   // +0x28 Array<Item*>*
    void* field_0x2c;                   // +0x2c Array<Ship*>*
    void* field_0x30;                   // +0x30 Array<Agent*>*
    uint8_t field_0x71;                 // +0x71

    // ---- methods (converted from free functions) ----
    void addItem(Item *item);
    void addShip(Ship *ship);
    Station * clone();
    Station * ctor(void *name, int p3, int p4, int p5, int p6);
    void ctor_default();
    void dtor();
    bool equals(Station *other);
    uint32_t getHiddenBlueprintIndex();
    RetStr getName();
    uint32_t getPirateStationIndex();
    uint8_t hasAttackedFriends();
    uint32_t hasItem(int index);
    uint32_t hasShip(int index);
    bool isAttackedByAliens();
    uint8_t isDiscovered();
    void removeShip(Ship *ship);
    void removeShips();
    void setAgents(void *agents);
    void setAttackedFriends(bool v);
    void setItems(uint32_t *items, bool deep);
    void setShips(uint32_t *ships, bool deep);
    uint32_t stationHasHiddenBlueprint(bool ignoreFound);
    uint32_t stationHasPirateBase();
    void visit();
};
#endif
