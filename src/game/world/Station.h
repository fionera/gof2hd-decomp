#ifndef GOF2_STATION_H
#define GOF2_STATION_H
#include "common.h"

class Item;
class Ship;
class Agent;

// Galaxy on Fire 2 - a trading station orbiting a planet within a solar system.
// Owns its for-sale items, the ships docked for sale, and the agents present.
class Station {
public:
    String name;
    int index;
    int systemIndex;
    uint8_t planet;
    int textureIndex;
    uint8_t visited;
    int techLevel;
    uint8_t attackedFriends;
    Array<Item*>* items;
    Array<Ship*>* ships;
    Array<Agent*>* agents;

    Station();
    Station(const String& name, int index, int systemIndex, int techLevel, int textureIndex);
    ~Station();

    Station* clone();

    void addItem(Item* item);
    void addShip(Ship* ship);
    void removeShip(Ship* ship);
    void removeShips();

    bool equals(Station* other);
    uint32_t hasItem(int index);
    uint32_t hasShip(int index);

    int getIndex() const { return index; }
    int getSystem() const { return systemIndex; }
    int getTecLevel() const { return techLevel; }
    int getTextureIndex() const { return textureIndex; }
    String getName() { return name; }

    Array<Agent*>* getAgents() const { return agents; }
    Array<Item*>* getItems() const { return items; }
    Array<Ship*>* getShips() const { return ships; }

    void setAgents(Array<Agent*>* agents);
    void setItems(Array<Item*>* items, bool deep);
    void setShips(Array<Ship*>* ships, bool deep);
    // Take ownership of a ready-built ship array (no deep clone) - used by save loading.
    void setShipsArr(Array<Ship*>* ships);

    void setAttackedFriends(bool v);
    uint8_t hasAttackedFriends();

    bool isAttackedByAliens();
    uint8_t isDiscovered();
    void visit();

    uint32_t getHiddenBlueprintIndex();
    uint32_t getPirateStationIndex();
    uint32_t stationHasHiddenBlueprint(bool ignoreFound);
    uint32_t stationHasPirateBase();

    // ArrayRemove<Ship*>: compact `ships`, dropping every slot equal to `ship`.
    static void arrayRemoveShip(Ship* ship, Array<Ship*>* ships);
};
#endif
