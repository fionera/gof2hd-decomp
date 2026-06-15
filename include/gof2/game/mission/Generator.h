#ifndef GOF2_GENERATOR_H
#define GOF2_GENERATOR_H

#include <cstdint>
#include "gof2/common.h"

// Forward declarations for the cross-class pointer types the Generator works
// with. The Generator never reaches into their layouts, so a forward decl is
// enough here; the full definitions are pulled in by Generator.cpp.
class Agent;
class Item;
class Mission;
class Ship;
class SolarSystem;
class Station;
class Status;

// Genuine free helpers used by the generator that have no class home of their
// own (declared here so this header stays self-sufficient).
void Globals_getRandomName(AbyssEngine::String *out, int names, int race,
                           int male);
Mission *Mission_ctor_full(Mission *self, int type, AbyssEngine::String *name,
                           int *imageParts, int race, int reward,
                           int targetStation, int difficulty);

// Procedural content factory: spawns the trade goods, agents, missions, ship
// and item buy lists, and loot drops for a station. Generator keeps no state of
// its own - every method works off the engine globals and its arguments - so
// construction and destruction are trivial.
class Generator {
public:
    Generator();

    void computerTradeGoods(Station *station);
    int generateStationIndex(Array<SolarSystem *> *systems, int station);
    Array<Agent *> *createAgents(Station *station);
    Mission *createMission(Agent *agent, Array<SolarSystem *> *systems);
    Agent *createAgent(Station *station);
    Array<Ship *> *getShipBuyList(Station *station);
    Array<Item *> *getItemBuyList(Station *station);
    Array<int> *getLootList(int itemIndex, int amount);
    bool isKaamoSpecialItem(int item);
};

#endif
