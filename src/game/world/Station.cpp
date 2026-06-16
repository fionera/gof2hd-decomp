#include "game/world/Station.h"
#include "game/ship/Ship.h"
#include "game/world/Galaxy.h"
#include "game/mission/Mission.h"
#include "game/mission/Item.h"
#include "game/mission/Status.h"
#include "game/ship/Agent.h"
#include "game/core/String.h"

// Stations that hide a blueprint / host a pirate base. The discovery flags and the
// alien-attack target are reached through the game's persistent-state singleton
// (gStatus); these were previously opaque holders pointing at the same Status object.
static const int kHiddenBlueprints[5] = { 0, 0, 0, 0, 0 };
static const int kPirateStations[4] = { 0, 0, 0, 0 };

Station::Station()
    : name("Station"),
      index(-1),
      systemIndex(-1),
      planet(0),
      textureIndex(0),
      visited(0),
      techLevel(0),
      attackedFriends(0),
      items(nullptr),
      ships(nullptr),
      agents(nullptr) {}

Station::Station(const String& name, int index, int systemIndex, int techLevel, int textureIndex)
    : name(name),
      index(index),
      systemIndex(systemIndex),
      planet(0),
      textureIndex(textureIndex),
      visited(0),
      techLevel(techLevel),
      attackedFriends(0),
      items(nullptr),
      ships(nullptr),
      agents(nullptr) {}

Station::~Station() {
    if (ships != nullptr) {
        for (Ship* s : *ships) delete s;
        ships->clear();
        delete ships;
        ships = nullptr;
    }
    if (items != nullptr) {
        for (Item* it : *items) delete it;
        items->clear();
        delete items;
        items = nullptr;
    }
    if (agents != nullptr) {
        Mission* campaign = gStatus->getCampaignMissionPtr();
        Mission* freelance = gStatus->getFreelanceMission();
        Agent* campaignAgent = campaign != nullptr ? campaign->getAgent() : nullptr;
        Agent* freelanceAgent = freelance != nullptr ? freelance->getAgent() : nullptr;
        for (Agent* a : *agents) {
            if (a != nullptr && a != campaignAgent && a != freelanceAgent && !a->isStoryAgent())
                delete a;
        }
        delete agents;
        agents = nullptr;
    }
}

Station* Station::clone() {
    return new Station(name, index, systemIndex, techLevel, textureIndex);
}

void Station::addItem(Item* item) {
    if (items == nullptr) {
        items = new Array<Item*>();
    } else {
        for (uint32_t i = 0; i < items->size(); i++) {
            if ((*items)[i]->equals(item)) {
                (*items)[i]->addAmount(item->getAmount());
                return;
            }
        }
    }
    items->push_back(item);
}

void Station::addShip(Ship* ship) {
    if (ships == nullptr) {
        ships = new Array<Ship*>();
    } else {
        for (uint32_t i = 0; i < ships->size(); i++) {
            if ((*ships)[i]->equals(ship))
                return;
        }
    }
    ships->push_back(ship);
}

void Station::arrayRemoveShip(Ship* ship, Array<Ship*>* ships) {
    uint32_t kept = 0;
    for (uint32_t i = 0; i < ships->size(); i++) {
        Ship* cur = (*ships)[i];
        if (cur != ship)
            (*ships)[kept++] = cur;
    }
    ships->resize(kept);
}

void Station::removeShip(Ship* ship) {
    if (ships == nullptr)
        return;
    arrayRemoveShip(ship, ships);
}

void Station::removeShips() {
    if (ships != nullptr) {
        for (Ship* s : *ships) delete s;
        ships->clear();
        delete ships;
    }
    ships = nullptr;
}

bool Station::equals(Station* other) {
    return other != nullptr && index == other->index;
}

uint32_t Station::hasItem(int index) {
    if (items != nullptr) {
        for (uint32_t i = 0; i < items->size(); i++) {
            Item* it = (*items)[i];
            if (it != nullptr && it->getIndex() == index)
                return 1;
        }
    }
    return 0;
}

uint32_t Station::hasShip(int index) {
    if (ships != nullptr) {
        for (uint32_t i = 0; i < ships->size(); i++) {
            Ship* sh = (*ships)[i];
            if (sh != nullptr && sh->getIndex() == index)
                return 1;
        }
    }
    return 0;
}

void Station::setItems(Array<Item*>* items, bool deep) {
    if (this->items != nullptr)
        delete this->items;
    this->items = nullptr;
    if (items == nullptr || !deep) {
        this->items = items;
    } else {
        Array<Item*>* copy = new Array<Item*>();
        this->items = copy;
        copy->resize(items->size());
        for (uint32_t i = 0; i < items->size(); i++)
            (*copy)[i] = (*items)[i]->clone();
    }
}

void Station::setShips(Array<Ship*>* ships, bool deep) {
    if (this->ships != nullptr) {
        for (Ship* s : *this->ships) delete s;
        this->ships->clear();
        delete this->ships;
    }
    this->ships = nullptr;
    if (ships == nullptr || !deep) {
        this->ships = ships;
    } else {
        Array<Ship*>* copy = new Array<Ship*>();
        this->ships = copy;
        copy->resize(ships->size());
        for (uint32_t i = 0; i < ships->size(); i++)
            (*copy)[i] = (*ships)[i]->clone();
    }
}

void Station::setShipsArr(Array<Ship*>* ships) {
    if (this->ships == ships)
        return;
    if (this->ships != nullptr) {
        for (Ship* s : *this->ships) delete s;
        this->ships->clear();
        delete this->ships;
    }
    this->ships = ships;
}

void Station::setAgents(Array<Agent*>* agents) {
    if (this->agents == agents)
        return;
    if (this->agents != nullptr) {
        for (Agent* a : *this->agents) delete a;
        this->agents->clear();
        delete this->agents;
    }
    this->agents = agents;
}

void Station::setAttackedFriends(bool v) {
    attackedFriends = v;
}

uint8_t Station::hasAttackedFriends() {
    return attackedFriends;
}

bool Station::isAttackedByAliens() {
    return index == *(int*)((char*)gStatus + 0x80);
}

uint8_t Station::isDiscovered() {
    return gGalaxy->getVisited()[index];
}

void Station::visit() {
    if (isDiscovered())
        return;
    visited = 1;
    gStatus->visitStation();
    gGalaxy->setSystemVisited(index);
}

uint32_t Station::getHiddenBlueprintIndex() {
    for (uint32_t i = 0; i < 5; i++) {
        if (kHiddenBlueprints[i] == index)
            return i;
    }
    return 0xffffffff;
}

uint32_t Station::getPirateStationIndex() {
    for (uint32_t i = 0; i < 4; i++) {
        if (kPirateStations[i] == index)
            return i;
    }
    return 0xffffffff;
}

uint32_t Station::stationHasHiddenBlueprint(bool ignoreFound) {
    char* base = (char*)gStatus;
    for (uint32_t i = 0; i < 5; i++) {
        if (kHiddenBlueprints[i] == index) {
            if (ignoreFound)
                return 1;
            char* flags = *(char**)(*(char**)(base + 0x58) + 4);
            if (flags[i] == 0)
                return 1;
        }
    }
    return 0;
}

uint32_t Station::stationHasPirateBase() {
    char* base = (char*)gStatus;
    for (uint32_t i = 0; i < 4; i++) {
        if (kPirateStations[i] == index) {
            char* flags = *(char**)(*(char**)(base + 0x4c) + 4);
            if (flags[i] == 0)
                return 1;
        }
    }
    return 0;
}
