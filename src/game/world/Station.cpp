#include "engine/core/ArrayReleaseClasses.h"
#include "game/world/Station.h"
#include "game/core/Globals.h"
#include "game/ship/Ship.h"
#include "game/world/Galaxy.h"
#include "game/mission/Mission.h"
#include "game/mission/Item.h"
#include "game/mission/Status.h"
#include "game/ship/Agent.h"
#include "game/core/String.h"

static const int kHiddenBlueprints[5] = {132, 133, 134, 129, 123};
static const int kPirateStations[4] = {1, 33, 47, 86};

Station::Station() {
    name = String("");
    index = -1;
    systemIndex = -1;
    techLevel = 0;
    textureIndex = 0;
    visited = 0;
    attackedFriends = 0;
    items = nullptr;
    ships = nullptr;
    agents = nullptr;
}

Station::Station(String name, int index, int systemIndex, int techLevel, int textureIndex) {
    this->name = name;
    this->index = index;
    this->systemIndex = systemIndex;
    this->techLevel = techLevel;
    this->textureIndex = textureIndex;
    this->visited = 0;
    this->attackedFriends = 0;
    this->items = nullptr;
    this->ships = nullptr;
    this->agents = nullptr;
}

Station::~Station() {
    if (ships != nullptr) {
        ArrayReleaseClasses(*ships);
        delete ships;
        ships = nullptr;
    }
    if (items != nullptr) {
        ArrayReleaseClasses(*items);
        delete items;
        items = nullptr;
    }
    if (agents != nullptr) {
        // Agents still referenced by the running missions and story agents are owned elsewhere.
        for (uint32_t i = 0; i < agents->size(); i++) {
            Agent *agent = (*agents)[i];
            Agent *campaignAgent = Globals::status->getCampaignMission() != 0
                                   ? ((Mission *) (intptr_t) Globals::status->getCampaignMission())->getAgent()
                                   : nullptr;
            Agent *freelanceAgent = Globals::status->getFreelanceMission() != nullptr
                                    ? Globals::status->getFreelanceMission()->getAgent()
                                    : nullptr;
            if (agent != nullptr && agent != campaignAgent && agent != freelanceAgent && !agent->isStoryAgent())
                delete agent;
        }
        delete agents;
        agents = nullptr;
    }
}

int Station::getIndex() { return index; }
int Station::getSystem() { return systemIndex; }
int Station::getTecLevel() { return techLevel; }
int Station::getTextureIndex() { return textureIndex; }
unsigned char Station::isPlanet() { return planet; }
String Station::getName() { return String(name, false); }

Array<Agent *> *Station::getAgents() { return agents; }
Array<Item *> *Station::getItems() { return items; }
Array<Ship *> *Station::getShips() { return ships; }

void Station::addItem(Item *item) {
    int index = -1;
    if (items != nullptr) {
        for (uint32_t i = 0; i < items->size(); i++) {
            if ((*items)[i]->equals(item)) {
                index = (int) i;
                break;
            }
        }
    } else {
        items = new Array<Item *>();
    }
    if (index >= 0)
        (*items)[index]->changeAmount(item->getAmount());
    else
        ArrayAdd(item, *items);
}

void Station::setAttackedFriends(bool v) {
    attackedFriends = v;
}

bool Station::hasAttackedFriends() {
    return attackedFriends;
}

bool Station::isAttackedByAliens() {
    return index == Globals::status->field_80;
}

uint8_t Station::isDiscovered() {
    return Globals::galaxy->getVisited()[index];
}

void Station::visit() {
    if (isDiscovered())
        return;
    visited = 1;
    Globals::status->visitStation();
    Globals::galaxy->visitStation(index);
}

void Station::removeShips() {
    if (ships != nullptr) {
        ArrayReleaseClasses(*ships);
        delete ships;
    }
    ships = nullptr;
}

void Station::removeShip(Ship *ship) {
    if (ships == nullptr)
        return;
    ArrayRemove(ship, *ships);
}

void Station::setShips(Array<Ship *> *ships, bool deep) {
    if (this->ships != nullptr) {
        ArrayReleaseClasses(*this->ships);
        delete this->ships;
    }
    this->ships = nullptr;
    if (ships == nullptr || !deep) {
        this->ships = ships;
    } else {
        Array<Ship *> *copy = new Array<Ship *>();
        this->ships = copy;
        ArraySetLength(ships->size(), *copy);
        for (uint32_t i = 0; i < ships->size(); i++)
            (*this->ships)[i] = (*ships)[i]->clone();
    }
}

void Station::setItems(Array<Item *> *items, bool deep) {
    if (this->items != nullptr)
        delete this->items;
    this->items = nullptr;
    if (items == nullptr || !deep) {
        this->items = items;
    } else {
        Array<Item *> *copy = new Array<Item *>();
        this->items = copy;
        ArraySetLength(items->size(), *copy);
        for (uint32_t i = 0; i < items->size(); i++)
            (*this->items)[i] = (*items)[i]->clone();
    }
}

void Station::setAgents(Array<Agent *> *agents) {
    if (this->agents == agents)
        return;
    if (this->agents != nullptr) {
        ArrayReleaseClasses(*this->agents);
        delete this->agents;
    }
    this->agents = agents;
}

uint32_t Station::hasItem(int index) {
    if (items != nullptr) {
        for (uint32_t i = 0; i < items->size(); i++) {
            Item *it = (*items)[i];
            if (it != nullptr && it->getIndex() == index)
                return 1;
        }
    }
    return 0;
}

uint32_t Station::hasShip(int index) {
    if (ships != nullptr) {
        for (uint32_t i = 0; i < ships->size(); i++) {
            Ship *sh = (*ships)[i];
            if (sh != nullptr && sh->getIndex() == index)
                return 1;
        }
    }
    return 0;
}

void Station::addShip(Ship *ship) {
    int index = -1;
    if (ships != nullptr) {
        for (uint32_t i = 0; i < ships->size(); i++) {
            if ((*ships)[i]->equals(ship)) {
                index = (int) i;
                break;
            }
        }
    } else {
        ships = new Array<Ship *>();
    }
    if (index < 0)
        ArrayAdd(ship, *ships);
}

uint32_t Station::stationHasPirateBase() {
    for (uint32_t i = 0; i < 4; i++) {
        if (kPirateStations[i] == index) {
            unsigned char *flags =
                reinterpret_cast<unsigned char *>(Globals::status->field_4c->data_);
            if (flags[i] == 0)
                return 1;
        }
    }
    return 0;
}

uint32_t Station::getPirateStationIndex() {
    for (uint32_t i = 0; i < 4; i++) {
        if (kPirateStations[i] == index)
            return i;
    }
    return 0xffffffff;
}

uint32_t Station::stationHasHiddenBlueprint(bool ignoreFound) {
    for (uint32_t i = 0; i < 5; i++) {
        if (kHiddenBlueprints[i] == index) {
            if (ignoreFound)
                return 1;
            unsigned char *flags =
                reinterpret_cast<unsigned char *>(Globals::status->field_58->data_);
            if (flags[i] == 0)
                return 1;
        }
    }
    return 0;
}

uint32_t Station::getHiddenBlueprintIndex() {
    for (uint32_t i = 0; i < 5; i++) {
        if (kHiddenBlueprints[i] == index)
            return i;
    }
    return 0xffffffff;
}

Station *Station::clone() {
    return new Station(String(name, false), index, systemIndex, techLevel, textureIndex);
}

bool Station::equals(Station *other) {
    return other != nullptr && index == other->index;
}
