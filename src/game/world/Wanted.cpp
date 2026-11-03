#include "game/world/Wanted.h"
#include "game/core/String.h"

Wanted::Wanted(int index, String name, int board, int race, bool male,
               int shipId, int weapon, int hitpoints, int lootItemId, int lootAmount,
               int reward, int requiredBounties, int requiredMission, int numWingmen) {
    this->index = index;
    this->name = name;
    this->board = board;
    this->race = race;
    this->male = male;
    this->terminated = false;
    this->active = false;
    this->lastSeen = -1;
    this->travelsTo = -1;
    this->currentLocation = -1;
    this->shipId = shipId;
    this->weapon = weapon;
    this->hitpoints = hitpoints;
    this->lootItemId = lootItemId;
    this->lootAmount = lootAmount;
    this->reward = reward;
    this->requiredBounties = requiredBounties;
    this->requiredMission = requiredMission;
    this->numWingmen = numWingmen;
}

Wanted::~Wanted() {
    delete[] imageParts;
    imageParts = nullptr;
}

int Wanted::getIndex() { return index; }

String Wanted::getName() {
    return String(name, false);
}

int Wanted::getNumWingmen() { return numWingmen; }
int Wanted::getBoard() { return board; }
int Wanted::getRace() { return race; }
bool Wanted::isMale() { return male; }
int Wanted::getShip() { return shipId; }
int Wanted::getWeapon() { return weapon; }
int Wanted::getHitpoints() { return hitpoints; }
int Wanted::getLoot() { return lootItemId; }
int Wanted::getLootAmount() { return lootAmount; }
int Wanted::getReward() { return reward; }
int Wanted::getRequiredBounties() { return requiredBounties; }
int Wanted::getRequiredMission() { return requiredMission; }

void Wanted::setRequiredMission(int v) { requiredMission = v; }
void Wanted::setImageParts(int *parts) { imageParts = parts; }
int *Wanted::getImageParts() { return imageParts; }
void Wanted::setTerminated(bool v) { terminated = v; }
void Wanted::setActive(bool v) { active = v; }
void Wanted::setTravelsTo(int v) { travelsTo = v; }
void Wanted::setCurrentLocation(int v) { currentLocation = v; }
void Wanted::setLastSeen(int v) { lastSeen = v; }

bool Wanted::isActive() { return active; }
bool Wanted::isTerminated() { return terminated; }
int Wanted::getLastSeen() { return lastSeen; }
int Wanted::getTravelsTo() { return travelsTo; }
int Wanted::getCurrentLocation() { return currentLocation; }
