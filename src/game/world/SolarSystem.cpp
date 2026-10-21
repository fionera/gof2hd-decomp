#include "game/world/SolarSystem.h"
#include "game/core/Globals.h"
#include "game/world/Galaxy.h"
#include "game/world/Station.h"
#include "game/mission/Status.h"
#include "game/core/String.h"

static const int kPirateBaseStations[4] = {1, 33, 47, 86};

static const int kAttackRaceTable[4] = {0, 0, 0, 0};

static const int kBlueprintStations[5] = {132, 133, 134, 129, 123};


SolarSystem::SolarSystem(int unk0, String displayName, int security,
                         bool isVisible, int factionId, int x, int y, int z,
                         int jumpgateId, int texture, int *starRGB,
                         Array<int> *stations, Array<int> *linkedSystems,
                         Array<int> *forbidden) {
    this->systemId = unk0;
    this->name = String(displayName, false);
    this->securityLevel = security;
    this->visible = isVisible;
    this->faction = factionId;
    this->mapX = x;
    this->mapY = y;
    this->mapZ = z;
    this->jumpgateStationId = jumpgateId;
    this->textureIndex = texture;
    this->starR = starRGB[0];
    this->starG = starRGB[1];
    this->starB = starRGB[2];
    this->stationIds = stations;
    this->forbiddenGoods = forbidden;
    this->linkedSystemIds = linkedSystems;
}

SolarSystem::~SolarSystem() {
    if (this->stationIds != nullptr) {
        ArrayRelease(*(this->stationIds));
        delete this->stationIds;
    }
    this->stationIds = nullptr;
    if (this->forbiddenGoods != nullptr) {
        ArrayRelease(*(this->forbiddenGoods));
        delete this->forbiddenGoods;
    }
    this->forbiddenGoods = nullptr;
    if (this->linkedSystemIds != nullptr) {
        ArrayRelease(*(this->linkedSystemIds));
        delete this->linkedSystemIds;
    }
    this->linkedSystemIds = nullptr;
}

int SolarSystem::getIndex() { return systemId; }
int SolarSystem::getRace() { return (int) faction; }
int SolarSystem::getSecurityLevel() { return securityLevel; }
int SolarSystem::getTextureIndex() { return textureIndex; }
int SolarSystem::getX() { return mapX; }
int SolarSystem::getY() { return mapY; }
int SolarSystem::getZ() { return mapZ; }
int SolarSystem::getWarpGateIndex() { return jumpgateStationId; }
uint32_t *SolarSystem::getStations() { return (uint32_t *) stationIds; }
uint32_t *SolarSystem::getRoutes() { return (uint32_t *) linkedSystemIds; }
void *SolarSystem::getForbiddenGoods() { return forbiddenGoods; } // lint: void_ptr (method return type — rule 3 forbids changing ClassName::method return types)
// lint: void_ptr (method return type — rule 3 forbids changing ClassName::method return types)

bool SolarSystem::isVisible() {
    return this->visible;
}

String SolarSystem::getName() {
    return this->name;
}

int SolarSystem::getAttackRace() {
    uint32_t r = this->faction;
    if (r < 4)
        return kAttackRaceTable[r];
    return 8;
}

int SolarSystem::hasPirateBase() {
    for (uint32_t i = 0; i < 4; i++) {
        if (stationIsInSystem(kPirateBaseStations[i]) && !(*Globals::status->field_4c)[i])
            return 1;
    }
    return 0;
}

int SolarSystem::stationIsInSystem(int idx) {
    Array<int> *arr = this->stationIds;
    uint32_t n = arr->size();
    for (uint32_t i = 0; i < n; i++) {
        if ((*arr)[i] == idx)
            return 1;
    }
    return 0;
}

int SolarSystem::hasHiddenBlueprint() {
    for (uint32_t i = 0; i < 5; i++) {
        if (stationIsInSystem(kBlueprintStations[i]) && !(*Globals::status->field_58)[i])
            return 1;
    }
    return 0;
}

int SolarSystem::getWarpGateEnumIndex() {
    return (int) getStationEnumIndex(this->jumpgateStationId);
}

uint32_t SolarSystem::getStationEnumIndex(int idx) {
    Array<int> *arr = this->stationIds;
    for (uint32_t i = 0; i < arr->size(); i++) {
        if ((*arr)[i] == idx)
            return i;
    }
    return 0xffffffff;
}

bool SolarSystem::currentOrbitHasWarpGate() {
    int orbit = this->jumpgateStationId;
    Station *st = (Globals::status)->getStation();
    return orbit == st->getIndex();
}

int SolarSystem::stationIsInSystem(Station *station) {
    if (station == nullptr)
        return 0;
    return stationIsInSystem(station->getIndex());
}

int SolarSystem::isFullyDiscovered() {
    for (uint32_t i = 0; i < this->stationIds->size(); i++) {
        if (!Globals::galaxy->getVisited()[(*this->stationIds)[i]])
            return 0;
    }
    return 1;
}

void SolarSystem::setCoords(int x, int y) {
    this->mapX = x;
    this->mapY = y;
}

bool SolarSystem::systemIsInSystemRoutes(int sys) {
    if (this->systemId != sys) {
        Array<int> *arr = this->linkedSystemIds;
        if (arr == nullptr)
            return false;
        uint32_t n = arr->size();
        for (uint32_t i = 0; i < n; i++) {
            if ((*arr)[i] == sys)
                return true;
        }
        return false;
    }
    return true;
}

void SolarSystem::setVisible(bool v) {
    this->visible = v;
}

uint32_t SolarSystem::hasNoOwner() {
    uint32_t x = this->systemId - 0x17;
    if (x < 0xb)
        return (0x60bU >> (x & 0x7ff)) & 1;
    return 0;
}
