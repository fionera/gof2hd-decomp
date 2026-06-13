#ifndef GOF2_STATION_H
#define GOF2_STATION_H
#include "gof2/common.h"
#include "gof2/game/ship/Agent.h"   // provides the shared RetStr aggregate
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — Station class. Top-level, no namespace.
// Field offsets recovered from per-method target disassembly. Methods access
// fields via byte-offset casts from `this`.


struct Station;

// RetStr (AbyssEngine::String by value: 12-byte trivially-copied aggregate)
// is provided by gof2/Agent.h (included above) to avoid a duplicate definition.

class Station {
public:
    int index;                      // +0xc
    int systemIndex;                     // +0x10
    int textureIndex;                     // +0x18
    uint8_t visited;                 // +0x1c
    int techLevel;                     // +0x20
    uint8_t attackedFriends;                 // +0x24
    void* items;                   // +0x28 Array<Item*>*
    void* ships;                   // +0x2c Array<Ship*>*
    void* agents;                   // +0x30 Array<Agent*>*
    uint8_t field_0x71;                 // +0x71

    // ---- methods (converted from free functions) ----
    void addItem(Item *item);
    void addShip(Ship *ship);
    int getIndex() const { return index; }
    int getSystem() const { return systemIndex; }
    int getTecLevel() const { return techLevel; }
    int getTextureIndex() const { return textureIndex; }
    void *getAgents() const { return agents; }
    void *getItems() const { return items; }   // +0x28 Array<Item*>*
    void *getShips() const { return ships; }    // +0x2c Array<Ship*>*
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
    // Take ownership of a ready-built ship array (no deep clone) — used by save loading.
    void setShipsArr(void *ships);
    // ArrayRemove<Ship*>: compact `ships`, dropping every slot equal to `ship`.
    static void arrayRemoveShip(Ship *ship, void *ships);
    // Destroy the AbyssEngine::String name base sub-object (last step of ~Station).
    void baseDtor();
    // Deleting-destructor tail: release the object's storage after ~Station has run.
    void dtorFinish();
    uint32_t stationHasHiddenBlueprint(bool ignoreFound);
    uint32_t stationHasPirateBase();
    void visit();
};
#endif
