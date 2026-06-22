#ifndef GOF2_GENERATOR_H
#define GOF2_GENERATOR_H

#include <cstdint>
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class Agent;
class Item;
class Mission;
class Ship;
class SolarSystem;
class Station;
class Status;

Mission *Mission_ctor_full(Mission *self, int type, AbyssEngine::String *name,
                           int *imageParts, int race, int reward,
                           int targetStation, int difficulty);

class Generator {
public:
    Generator();

    ~Generator();

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
