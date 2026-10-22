#include "game/mission/Status.h"
#include <cstring>
#include "game/core/Globals.h"
#include "game/mission/PendingProduct.h"
#include "game/core/GameSettings.h"
#include "engine/core/AERandom.h"
#include "game/world/Galaxy.h"
#include "game/mission/Item.h"
#include "game/mission/Achievements.h"
#include "game/ship/Agent.h"
#include "game/mission/BluePrint.h"
#include "game/mission/Mission.h"
#include "game/world/Station.h"
#include "game/world/SolarSystem.h"
#include "game/world/Wanted.h"
#include "game/world/Standing.h"
#include "game/ship/Ship.h"
#include "engine/file/FileRead.h"
#include "game/mission/Generator.h"
#include "game/world/SystemPathFinder.h"
#include "game/world/Level.h"

static SolarSystem *asSystem(int32_t v) { return (SolarSystem *) (intptr_t) v; }

int Status::getPlanetNames() { return planetNames; }

int Status::getJumpgateUsed() { return jumpgatesUsed; }

bool Status::dlc1Won() { return 0x53 < currentCampaignMission; }

void Status::incEquipmentBought() { boughtEquipment = boughtEquipment + 1; }

void Status::setKills(int v) { kills = v; }

void Status::incMissionCount() { missionCount = missionCount + 1; }

void Status::incPlayingTime(int64_t delta) { playingTime = playingTime + delta; }

uint32_t Status::orbitHasPlanetRing(int index) {
    unsigned t = index - 0x78U;
    if (((t >> 1) | (t << 0x1f)) < 7) {
        return 0x69U >> (((index - 0x78U) >> 1) & 0x7f) & 1;
    }
    return 0;
}

int Status::getSystem() { return system; }

bool Status::inSupernovaSystem() {
    if (inAlienOrbit()) return 0;
    return asSystem(system)->getIndex() == 0x1b && currentCampaignMission < 0x9e;
}

void Status::visitStation() { stationsVisited = stationsVisited + 1; }

int Status::getMaxMissions() { return 2; }

void Status::setPirateKills(int v) { pirateKills = v; }

void Status::setFreelanceMission(Mission *m) {
    (*missions)[1] = m;
}

Array<Station *> *Status::getStationStack() {
    return stationStack;
}

Mission *Status::getMission() { return mission; }

void Status::setPassengers(int p) { reinterpret_cast<int32_t &>(_byte_0x34) = p; }

int Status::getMissionCount() { return missionCount; }

void Status::setCredits(int v) { credits = v; }

bool Status::stringHasToken(String haystack, String needle) {
    return (int) haystack.IndexOf(needle) > -1;
}

int Status::getLastXP() { return lastXP; }

void Status::setLastXP(int v) { lastXP = v; }

void Status::setStationsVisited(int v) { stationsVisited = v; }

void Status::setStationStack(Array<Station *> *stack) { stationStack = stack; }

struct DifficultyOptionView {
    uint8_t pad[0x2c];
    float difficulty;
};

bool Status::hardCoreMode() {
    return reinterpret_cast<DifficultyOptionView *>(Globals::options)->difficulty == 1.5f;
}

void Status::crateCaptured(int delta) { capturedCrates = delta + capturedCrates; }

int Status::getStanding() { return (int) (intptr_t) standing; }

int Status::getBoughtEquipment() { return boughtEquipment; }

int Status::getCredits() { return credits; }

void Status::setRating(int v) { rating = v; }

Array<Wanted *> *Status::getWanted() { return wanted; }

int Status::getCurrentCampaignMission() { return currentCampaignMission; }

void Status::setMissionCount(int v) { missionCount = v; }

void Status::setShip(Ship *s) {
    if (ship != 0) {
        delete ship;
        ship = 0;
    }
    ship = s;
}

Array<Mission *> *Status::getMissions() { return missions; }

bool Status::inFogSkyboxOrbit() {
    if (inAlienOrbit()) return false;
    if (asSystem(system)->getTextureIndex() == 0x11) return true;
    return asSystem(system)->getTextureIndex() == 0x12;
}

bool Status::inPirateLootOrbit() {
    if (inAlienOrbit()) return false;
    if (asSystem(system)->getIndex() == 0x20) return true;
    return asSystem(system)->getIndex() == 0x21;
}

void Status::setPlayingTime(int64_t v) { playingTime = v; }

Array<PendingProduct *> *Status::getPendingProducts() { return pendingProducts; }

Array<bool> *Status::getSystemVisibilities() { return systemVisibilities; }

void Status::setSystemVisibility(int index, bool value) {
    (*systemVisibilities)[index] = value;
}

void Status::incCollectedBounties(int index) {
    if (index < 4) {
        collectedBounties[index] = collectedBounties[index] + 1;
    }
}

void Status::setCurrentCampaignMission(int v) { currentCampaignMission = v; }

void Status::addKills(int delta) { kills = delta + kills; }

int Status::getPirateKills() { return pirateKills; }

Station *Status::getStation() { return station; }

void Status::setCampaignMission(Mission *m) {
    m->setCampaignMission(true);
    Mission *cur = missions->data()[0];
    if (cur != 0 && cur != Mission::empty) {
        delete cur;
        missions->data()[0] = 0;
    }
    missions->data()[0] = m;
}

Wanted *Status::getWantedInCurrentOrbit() {
    Wanted *best = 0;
    if (wanted != 0) {
        for (unsigned i = 0; i < wanted->size(); i = i + 1) {
            if ((*wanted)[i]->isActive() && !(*wanted)[i]->isTerminated() &&
                (*wanted)[i]->getCurrentLocation() == station->getIndex()) {
                if (best == 0 || best->getRequiredBounties() < (*wanted)[i]->getRequiredBounties()) {
                    best = (*wanted)[i];
                }
            }
        }
    }
    return best;
}

int Status::getLevel() { return level; }

Status::~Status() {
    if (ship != 0) {
        delete ship;
    }
    ship = 0;
    if (mission != 0) {
        delete mission;
    }
    mission = 0;
    if (agents != 0) {
        ArrayReleaseClasses(*agents);
        delete agents;
    }
    agents = 0;
    if (stationStack != 0) {
        ArrayReleaseClasses(*stationStack);
        delete stationStack;
    }
    stationStack = 0;
    if (wanted != 0) {
        ArrayReleaseClasses(*wanted);
        delete wanted;
    }
    wanted = 0;
}

bool Status::inStormOrbit() {
    if (!inAlienOrbit()) {
        if (currentCampaignMission >= 0x5a) {
            if (inSupernovaSystem() != 0) return true;
            if (asSystem(system)->getTextureIndex() == 0x10) return true;
            return asSystem(system)->getTextureIndex() == 0x12;
        }
    }
    return false;
}

int Status::getCapturedCrates() { return capturedCrates; }

void Status::incKills() {
    kills = kills + 1;
    Globals::achievements->incKills();
}

int Status::getStationsVisited() { return stationsVisited; }

int Status::getCampaignMission() {
    return (int) (intptr_t)(*missions)[0];
}

void Status::setJumpgateUsed(int v) { jumpgatesUsed = v; }

bool Status::gameWon() { return 0x2c < currentCampaignMission; }

void Status::setLevel(int v) { level = v; }

void Status::setCapturedCrates(int v) { capturedCrates = v; }

bool Status::inBlackMarketSystem() {
    if (inAlienOrbit()) return false;
    return asSystem(system)->getIndex() == 0x19;
}

void Status::incGoodsProduced(int delta) { goodsProduced = delta + goodsProduced; }

int64_t Status::getPlayingTime() { return playingTime; }

int Status::getNumberOfMissions() {
    if (missions == 0) {
        return 0;
    }
    int count = 0;
    for (unsigned i = 0; i < missions->size(); i = i + 1) {
        if ((*missions)[i] != 0) {
            count = count + 1;
        }
    }
    return count;
}

bool Status::inSupernovaOrbit() {
    if (inAlienOrbit()) return false;
    return station->getIndex() == 0x6d;
}

bool Status::inDeepScienceOrbit() {
    if (inAlienOrbit()) return false;
    if (station->getIndex() == 10) return true;
    return station->getIndex() == 100;
}

int Status::wantedBoardAccessible() {
    Array<Wanted *> *w = Globals::status->wanted;
    for (unsigned i = 0; i < w->size(); i = i + 1) {
        if (asSystem(Globals::status->system)->getRace() == (*w)[i]->getBoard() &&
            Globals::status->currentCampaignMission >= (*w)[i]->getRequiredMission() &&
            Globals::status->inAlienOrbit() == 0 &&
            Globals::status->station->getIndex() != 0x6c) {
            return 1;
        }
    }
    return 0;
}

void Status::incPirateKills() {
    pirateKills = pirateKills + 1;
    Globals::achievements->incPirateKills();
}

void Status::setGoodsProduced(int v) { goodsProduced = v; }

void Status::setMission(Mission *m) { mission = m; }

int Status::getCollectedBounties(int index) {
    if (index < 4) return collectedBounties[index];
    return 0;
}

int Status::getRating() { return rating; }

Mission *Status::getFreelanceMission() {
    return (*missions)[1];
}

void Status::jumpgateUsed() { jumpgatesUsed = jumpgatesUsed + 1; }

Ship *Status::getShip() { return ship; }

int Status::getPlanetTextures() { return planetTextures; }

int Status::getPassengers() { return reinterpret_cast<int32_t &>(_byte_0x34); }

bool Status::isStorylineWanted(int index) { return (unsigned) index < 2; }

int Status::missionFailed(bool docked, int64_t time) {
    for (unsigned i = 0; i < missions->size(); i = i + 1) {
        Mission *cur = (*missions)[i];
        if (cur->hasFailed() != 0) {
            return 0;
        }
        if (cur != 0 && cur->getType() == 0xd && docked && this->field_0xf1 != 0) {
            return (int) (intptr_t) cur;
        }
    }
    return 0;
}

int Status::getBluePrints() { return (int) (intptr_t) bluePrints; }

int Status::getAgents() { return (int) (intptr_t) agents; }

uint32_t Status::inPlanetRingOrbit() {
    if (inAlienOrbit() == 0) {
        int idx = station->getIndex();
        unsigned t = idx - 0x78U;
        if (((t >> 1) | (t << 0x1f)) < 7) {
            return 0x69U >> (((idx - 0x78U) >> 1) & 0x7f) & 1;
        }
    }
    return 0;
}

void Status::setBoughtEquipment(int v) { boughtEquipment = v; }

int Status::getWingmen() { return (int) (intptr_t) wingmen; }

int Status::getGoodsProduced() { return goodsProduced; }

int Status::getKills() { return kills; }

void Status::changeCredits(int delta) {
    int magnitude = delta < 0 ? -delta : delta;
    if (magnitude <= 1000000000) {
        int credited = delta + credits;
        credits = credited & ~(credited >> 0x1f);
    }
}


int Status::isFreighterMissionStation(int station) {
    switch (station) {
        case 15:
        case 30:
        case 40:
        case 45:
        case 60:
        case 70:
        case 80:
        case 85:
        case 95:
            return 1;
        default:
            return 0;
    }
}

bool Status::isBlueprintUnlocked(int index) {
    for (unsigned i = 0; i < bluePrints->size(); i++) {
        if ((*bluePrints)[i]->getIndex() == index) {
            return (*bluePrints)[i]->isUnlocked();
        }
    }
    return false;
}

Status::Status() {
    missions = new Array<Mission *>();
    stationStack = new Array<Station *>();
    systemVisibilities = new Array<bool>();
    field_94 = new Array<bool>();
    field_98 = new Array<bool>();
    field_ac = new Array<bool>();
    field_b4 = new Array<bool>();
    field_4c = new Array<bool>();
    field_50 = new Array<bool>();
    field_90 = new Array<int>();
    field_54 = new Array<bool>();
    field_58 = new Array<bool>();
    ArraySetLength(2, *missions);
    ArraySetLength(3, *stationStack);
    ArraySetLength(0x22, *systemVisibilities);
    ArraySetLength(0xb, *field_94);
    ArraySetLength(0xb, *field_98);
    ArraySetLength(0x16, *field_ac);
    ArraySetLength(0x22, *field_b4);
    ArraySetLength(4, *field_4c);
    ArraySetLength(0xf, *field_50);
    ArraySetLength(0xe9, *field_54);
    ArraySetLength(5, *field_58);
    currentCampaignMission = 0;
    stationsVisited = 0;
    reinterpret_cast<int32_t &>(_byte_0x34) = 0;
    field_10c = 0;
    field_110 = 0;
    field_0x111 = 0;
    kills = 0;
    missionCount = 0;
    level = 1;
    credits = 0;
    rating = 0;
    playingTime = 0;
    playerStation = new Station();
    ship = 0;
    field_8c = 0;
    mission = 0;
    station = 0;
    system = 0;
    planetNames = 0;
    planetTextures = 0;
    voidStation = nullptr;
    wanted = 0;
    wingmen = 0;
    reinterpret_cast<int32_t &>(this->_byte_0x28) = 0;
    standing = 0;
    bluePrints = 0;
    pendingProducts = 0;
    agents = 0;
    this->field_0x3c = 0;
    this->field_0x40 = 0;
    this->field_0x44 = 0;
    this->field_0x48 = 0;
    for (int i = 0; i < 4; i++) {
        collectedBounties[i] = 0;
    }
    field_178 = 0;
    this->field_0x17c = 0;
}

int Status::getFreighterMissionStationBit(int station) {
    switch (station) {
        case 85: return 1;
        case 30: return 2;
        case 40: return 3;
        case 15: return 4;
        case 60: return 5;
        case 95: return 6;
        case 70: return 7;
        case 80: return 8;
        case 45: return 9;
        default: return 0;
    }
}

void Status::calcCargoPrices() {
    Array<SolarSystem *> *systems = Globals::galaxy->getSystems();
    for (int list = 0; list != 3; list++) {
        Array<Item *> *items;
        switch (list) {
            case 0:
                items = this->ship->getCargo();
                break;
            case 1:
                items = this->ship->getEquipment();
                break;
            default:
                items = this->station->getItems();
                break;
        }
        if (items == 0) {
            continue;
        }
        Globals::rnd->setSeed((long long) this->station->getIndex());
        bool ringWorld;
        if (Globals::status->inAlienOrbit()) {
            ringWorld = false;
        } else {
            ringWorld = asSystem(Globals::status->system)->getIndex() == 25;
        }
        for (unsigned i = 0; i < items->size(); i++) {
            Item *item = (*items)[i];
            if (item == 0) {
                continue;
            }
            int priceRange = Globals::galaxy->distancePercent(
                (*systems)[item->getMinPriceSystem()]->getX(), (*systems)[item->getMinPriceSystem()]->getY(),
                (*systems)[item->getMaxPriceSystem()]->getX(), (*systems)[item->getMaxPriceSystem()]->getY());
            int distance = Globals::galaxy->distancePercent(
                (*systems)[(*items)[i]->getMinPriceSystem()]->getX(),
                (*systems)[(*items)[i]->getMinPriceSystem()]->getY(),
                (*systems)[this->station->getSystem()]->getX(), (*systems)[this->station->getSystem()]->getY());
            float factor = 100.0f / (float) priceRange * (float) distance / 100.0f;
            int minPrice = item->getMinPrice();
            int priceBonus = (int) ((factor < 1.0f ? factor : 1.0f) * (float) (item->getMaxPrice() - item->getMinPrice()));
            if (item->getSinglePrice() < 1) {
                continue;
            }
            int price;
            if (ringWorld) {
                price = item->getMaxPrice();
            } else {
                int base = minPrice + priceBonus;
                int jitter = (int) ((float) base * 0.02f);
                if (jitter <= 1) {
                    jitter = 1;
                }
                price = base - jitter + Globals::rnd->nextInt(jitter << 1 | 1);
            }
            if (Globals::globalPriceRaise != 0) {
                price = (int) ((float) price + (float) price * (float) Globals::globalPriceRaise * 0.01f);
            }
            item->setPrice(price);
        }
        Globals::rnd->reset();
    }
}

Station *Status::isOnStack(Station *s) {
    unsigned i = 0;
    while (true) {
        if (2 < i) {
            return 0;
        }
        Station *cur = (*stationStack)[i];
        if (cur != 0 && cur->equals(s) != 0) {
            break;
        }
        i = i + 1;
    }
    return (*stationStack)[i];
}

bool Status::inAlienOrbit() {
    return station->equals(playerStation);
}

void Status::resetGame() {
    GameSettings *settings = reinterpret_cast<GameSettings *>(Globals::options);
    this->boughtEquipment = 0;
    this->field_8c = 0;
    this->field_0x100 = 0;
    this->field_0x104 = 0;
    this->credits = 0;
    this->rating = 0;
    this->kills = 0;
    this->missionCount = 0;
    this->stationsVisited = 0;
    reinterpret_cast<int64_t &>(this->level) = 1;
    this->playingTime = 0;
    this->field_10c = 0;
    this->field_110 = 0;
    this->field_118 = 0;
    this->field_11c = 0;
    this->field_124 = 0;
    this->field_12c = 0;
    this->goodsProduced = 0;
    this->pirateKills = 0;
    this->jumpgatesUsed = 0;
    this->capturedCrates = 0;
    Station *oldStation = this->voidStation;
    this->field_13c = 0;
    this->field_134 = 0;
    this->field_144 = 0;
    this->field_120 = 0;
    this->field_128 = 0;
    this->field_130 = 0;
    this->field_138 = 0;
    this->field_140 = 0;
    this->field_148 = 0;
    this->field_114 = settings->hardCoreFlag != 0 ? 3 : 0;
    this->field_0x111 = 0;

    if (oldStation != nullptr) {
        delete oldStation;
        this->voidStation = nullptr;
    }
    this->voidStation = new Station();

    if (reinterpret_cast<int32_t &>(this->_byte_0x28) != 0) {
        delete[] (char *) (intptr_t) reinterpret_cast<int32_t &>(this->_byte_0x28);
        reinterpret_cast<int32_t &>(this->_byte_0x28) = 0;
    }
    if (this->wingmen != 0) {
        ArrayReleaseClasses(*this->wingmen);
        if (this->wingmen != 0) {
            delete this->wingmen;
        }
    }
    this->wingmen = 0;
    this->fadeValue = 0;

    for (unsigned j = 0; j < this->field_94->size(); j++)
        (*this->field_94)[j] = 0;
    for (unsigned j = 0; j < this->field_98->size(); j++)
        (*this->field_98)[j] = 0;
    this->field_9c = 0;
    this->field_a0 = 0;
    this->field_a4 = 0;
    this->field_a8 = 0;
    for (unsigned j = 0; j < this->field_ac->size(); j++)
        (*this->field_ac)[j] = 0;
    this->field_b0 = 0;
    for (unsigned j = 0; j < this->field_b4->size(); j++)
        (*this->field_b4)[j] = 0;

    if (this->stationStack != 0) {
        for (unsigned k = 0; k < this->stationStack->size(); k++) {
            if ((*this->stationStack)[k] != 0) {
                delete (*this->stationStack)[k];
                (*this->stationStack)[k] = 0;
            }
        }
    }

    Globals::globals->resetHints();
    Globals::galaxy->reset();

    for (unsigned j = 0; j < this->field_50->size(); j++)
        (*this->field_50)[j] = 0;
    for (unsigned j = 0; j < 176; j++)
        (*this->field_54)[j] = 1;
    for (unsigned j = 176; j < this->field_54->size(); j++)
        (*this->field_54)[j] = 0;
    (*this->field_54)[62] = 0;
    (*this->field_54)[61] = 0;
    (*this->field_54)[60] = 0;

    this->field_b8 = 0;
    reinterpret_cast<int32_t &>(this->_byte_0x34) = 0;
    this->field_e0 = 0;
    this->field_e4 = 0;
    this->field_e8 = 0;
    this->field_ec = 0;
    this->field_c0 = 0;
    this->field_c4 = 0;
    this->field_c8 = 0;
    this->field_cc = 0;
    this->field_d0 = 0;
    this->field_d4 = 0;
    this->field_d8 = 0;
    this->field_dc = 0;
    Level::energyCellsForNextJump = 0;
    Level::doInstantJump = 0;
    Level::programmedStation = 0;
    reinterpret_cast<int64_t &>(this->field_160) = 0;
    this->field_0x108 = 0;
    this->field_174 = 0;
    __atomic_signal_fence(__ATOMIC_SEQ_CST);
    this->field_0xf0 = 0;
    this->field_0xf1 = 0;
    __atomic_signal_fence(__ATOMIC_SEQ_CST);
    this->field_f4 = -1;
    __atomic_signal_fence(__ATOMIC_SEQ_CST);
    this->field_f8 = 1;
    __atomic_signal_fence(__ATOMIC_SEQ_CST);
    Achievements *ach = Globals::achievements;
    ach->init();

    if (this->field_0x40 != 0) {
        delete this->field_0x40;
        this->field_0x40 = 0;
    }
    if (this->field_0x3c != 0) {
        delete this->field_0x3c;
        this->field_0x3c = 0;
    }
    if (this->field_0x48 != 0) {
        delete this->field_0x48;
        this->field_0x48 = 0;
    }
    if (this->field_0x44 != 0) {
        delete this->field_0x44;
        this->field_0x44 = 0;
    }
    this->field_0x40 = new Array<int>();
    ArraySetLength(233, *this->field_0x40);
    this->field_0x3c = new Array<int>();
    ArraySetLength(233, *this->field_0x3c);
    this->field_0x48 = new Array<int>();
    ArraySetLength(233, *this->field_0x48);
    this->field_0x44 = new Array<int>();
    ArraySetLength(233, *this->field_0x44);
    for (int j = 0; j < 233; j++) {
        (*this->field_0x40)[j] = 0;
        (*this->field_0x3c)[j] = 0;
        (*this->field_0x48)[j] = 0;
        (*this->field_0x44)[j] = 0;
    }

    if (this->field_4c != 0) {
        delete this->field_4c;
        this->field_4c = 0;
    }
    this->field_4c = new Array<bool>();
    ArraySetLength(4, *this->field_4c);
    for (int j = 0; j < 4; j++)
        (*this->field_4c)[j] = 0;

    if (this->field_58 != 0) {
        delete this->field_58;
        this->field_58 = 0;
    }
    this->field_58 = new Array<bool>();
    ArraySetLength(5, *this->field_58);
    for (int j = 0; j < 5; j++)
        (*this->field_58)[j] = 0;

    Array<SolarSystem *> *systems = Globals::galaxy->getSystems();
    Array<bool> *visibilities = this->systemVisibilities;
    for (unsigned j = 0; j < systems->size(); j++)
        (*visibilities)[j] = (*systems)[j]->isVisible();

    Array<Item *> *items = Globals::items;
    int blueprintCount = 0;
    for (unsigned j = 0; j < items->size(); j++) {
        if ((*items)[j]->getIngredients() != 0)
            blueprintCount++;
    }
    if (this->bluePrints != 0) {
        ArrayReleaseClasses(*this->bluePrints);
        if (this->bluePrints != 0) {
            delete this->bluePrints;
        }
    }
    this->bluePrints = 0;
    if (blueprintCount != 0) {
        this->bluePrints = new Array<BluePrint *>();
        ArraySetLength(blueprintCount, *this->bluePrints);
        int k = 0;
        for (unsigned j = 0; j < items->size(); j++) {
            if ((*items)[j]->getIngredients() != 0) {
                (*this->bluePrints)[k] = new BluePrint(j);
                k++;
            }
        }
    }
    if (this->pendingProducts != 0) {
        ArrayReleaseClasses(*this->pendingProducts);
        if (this->pendingProducts != 0) {
            delete this->pendingProducts;
        }
    }
    this->pendingProducts = 0;

    loadAgents();
    loadWanted();

    if (this->standing != 0) {
        delete this->standing;
        this->standing = 0;
    }
    this->standing = new Standing();
    this->field_7c = -1;
    this->field_80 = -1;
    this->field_84 = 0;
    (*this->missions)[1] = Mission::empty;
    this->currentCampaignMission = 0;
    setCampaignMission(new Mission(4, 0, 78));
    this->mission = (*this->missions)[0];

    setShip((*Globals::ships)[10]->makeShip(-1));
    this->ship->priceDecline();
    setStation((Station *) (intptr_t) Globals::galaxy->getStation(78));
    this->ship->setCargo(0);
    Item *item = (*Globals::items)[2]->makeItem();
    this->ship->setEquipment(item, 0);
    item = (*Globals::items)[2]->makeItem();
    this->ship->setEquipment(item, 1);
    item = (*Globals::items)[54]->makeItem();
    this->ship->setEquipment(item, 0);
    item = (*Globals::items)[59]->makeItem();
    this->ship->setEquipment(item, 1);
    item = (*Globals::items)[82]->makeItem();
    this->ship->setEquipment(item, 2);
    item = (*Globals::items)[73]->makeItem();
    this->ship->setEquipment(item, 3);
    item = (*Globals::items)[36]->makeItem(6);
    Globals::status->ship->setEquipment(item, 0);

    if (settings->blackMarketUnlockedFlag != 0)
        (*this->systemVisibilities)[25] = 1;

    this->field_64 = this->ship->getMaxHP();
    this->field_5c = this->ship->getMaxShieldHP();
    this->field_60 = this->ship->getMaxArmorHP();
    this->field_68 = 100;
    this->field_150 = -1;
    this->field_154 = -1;
    this->field_158 = -1;
}


int Status::activateNewWanted() {
    Array<Wanted *> *wanted = Globals::status->wanted;
    if (wanted == 0) {
        return 0;
    }
    SystemPathFinder *pathFinder = 0;
    Array<SolarSystem *> *systems = 0;
    int activated = 0;
    for (unsigned i = 0; i < wanted->size(); i++) {
        Wanted *cur = (*wanted)[i];
        if (cur == 0) {
            continue;
        }
        unsigned minJumps;
        unsigned maxJumps;
        if (i < 2) {
            maxJumps = 4;
            minJumps = 2;
        } else {
            int step = (int) (i - 1) % 6;
            minJumps = step / 3 + 2;
            maxJumps = 4 + step / 2;
        }
        if (asSystem(Globals::status->system)->getRace() != cur->getBoard()) {
            continue;
        }
        if (Globals::status->currentCampaignMission < cur->getRequiredMission()) {
            continue;
        }
        if (i < 2 && Globals::status->currentCampaignMission > cur->getRequiredMission()) {
            continue;
        }
        if (Globals::status->inAlienOrbit()) {
            continue;
        }
        if (Globals::status->station->getIndex() == 108) {
            continue;
        }
        if (cur->isActive()) {
            continue;
        }
        if (cur->isTerminated()) {
            continue;
        }
        if (Globals::status->getCollectedBounties(cur->getBoard()) < cur->getRequiredBounties()) {
            continue;
        }
        cur->setActive(true);
        activated++;
        if (pathFinder == 0) {
            pathFinder = new SystemPathFinder();
            FileRead *fileRead = new FileRead();
            systems = fileRead->loadSystemsBinary();
            delete fileRead;
        }
        Array<int> *path;
        do {
            Station *from = Globals::globals->getRandomStation();
            unsigned fromSystem = from->getSystem();
            Array<bool> *visibilities = Globals::status->systemVisibilities;
            bool visible = false;
            if (visibilities != 0 && visibilities->size() > fromSystem) {
                visible = (*visibilities)[fromSystem];
            }
            while ((*systems)[fromSystem]->getRoutes() == 0 || !visible || fromSystem == 27 || fromSystem == 28 ||
                   fromSystem == 25 || fromSystem == 26 || fromSystem == 6) {
                if (from != 0) {
                    delete from;
                }
                from = Globals::globals->getRandomStation();
                fromSystem = from->getSystem();
                visible = false;
                if (visibilities != 0 && visibilities->size() > fromSystem) {
                    visible = (*visibilities)[fromSystem];
                }
            }
            Station *to = Globals::globals->getRandomStation();
            unsigned toSystem = to->getSystem();
            visible = false;
            if (visibilities != 0 && visibilities->size() > toSystem) {
                visible = (*visibilities)[toSystem];
            }
            while ((*systems)[toSystem]->getRoutes() == 0 || !visible || toSystem == 27 || toSystem == 28 ||
                   toSystem == 25 || toSystem == 26 || toSystem == 6 || fromSystem == toSystem) {
                if (to != 0) {
                    delete to;
                }
                to = Globals::globals->getRandomStation();
                toSystem = to->getSystem();
                visible = false;
                if (visibilities != 0 && visibilities->size() > toSystem) {
                    visible = (*visibilities)[toSystem];
                }
            }
            cur->setLastSeen(from->getIndex());
            cur->setTravelsTo(to->getIndex());
            int fromIndex = from->getSystem();
            int toIndex = to->getSystem();
            if (from != 0) {
                delete from;
            }
            if (to != 0) {
                delete to;
            }
            path = pathFinder->getSystemPath(systems, fromIndex, toIndex);
        } while (path == 0 || path->size() < minJumps || path->size() > maxJumps);
        int target = (*path)[Globals::rnd->nextInt(path->size())];
        if ((*systems)[target]->getStations() != 0) {
            int pick = Globals::rnd->nextInt(
                reinterpret_cast<Array<int> *>((*systems)[target]->getStations())->size());
            cur->setCurrentLocation((*reinterpret_cast<Array<int> *>((*systems)[target]->getStations()))[pick]);
        }
        delete path;
    }
    if (pathFinder != 0) {
        delete pathFinder;
    }
    if (systems != 0) {
        ArrayReleaseClasses(*systems);
        delete systems;
    }
    return activated;
}

void Status::addPendingProduct(BluePrint *bp) {
    if (pendingProducts == 0) {
        pendingProducts = new Array<PendingProduct *>();
    } else {
        for (unsigned i = 0; i < pendingProducts->size(); i = i + 1) {
            if ((*pendingProducts)[i] != 0 && (*pendingProducts)[i]->blueprintIndex == bp->getIndex() &&
                (*pendingProducts)[i]->stationIndex == bp->getStationIndex()) {
                (*pendingProducts)[i]->quantity += bp->getQuantity();
                return;
            }
        }
    }
    PendingProduct *pp = new PendingProduct(bp);
    ArrayAdd(pp, *pendingProducts);
}

bool Status::inEmptyOrbit() {
    int idx = station->getIndex();
    if (currentCampaignMission < 2 && idx == 78) {
        return true;
    }
    if (inAlienOrbit() && currentCampaignMission >= 43 && currentCampaignMission <= 83) {
        return true;
    }
    if (inAlienOrbit() && currentCampaignMission > 153) {
        return true;
    }
    switch (idx) {
        case 102:
        case 103:
        case 104:
        case 109:
        case 110:
        case 132:
        case 133:
            return true;
        case 111:
            if (currentCampaignMission > 93) {
                return true;
            }
            break;
        case 101:
            if (currentCampaignMission > 83) {
                return true;
            }
            break;
    }
    return idx == 134;
}

void Status::loadWanted() {
    FileRead *fileRead = new FileRead();
    wanted = fileRead->loadWanted();
    delete fileRead;
}

void Status::unlockBluePrint(int index) {
    for (unsigned i = 0; i < bluePrints->size(); i = i + 1) {
        if ((*bluePrints)[i]->getIndex() == index) {
            (*bluePrints)[i]->unlock();
        }
    }
}

float Status::getGammaRayDamagePerSecond(int station, int system) {
    float damage = 0.0f;
    if ((unsigned) (station - 109) <= 4) {
        if (system <= 105) {
            switch (station) {
                case 109: return 0.7f;
                case 110: return 0.4f;
                case 111: return 0.4f;
                case 112: return 0.3f;
                case 113: return 0.2f;
            }
        } else if (currentCampaignMission <= 157) {
            switch (station) {
                case 109: return 3.0f;
                case 110: return 2.0f;
                case 111: return 1.0f;
                case 112: return 0.5f;
                case 113: return 0.3f;
            }
        } else if (station == 109) {
            damage = 1.0f;
        }
    }
    return damage;
}

int Status::addStationToStack(Station *s) {
    Station *found = isOnStack(s);
    if (found != 0) {
        setStation(found);
        return 0;
    }
    if ((*stationStack)[0] == 0) {
        for (int i = 2; i >= 0; i--) {
            if ((*stationStack)[i] == 0) {
                (*stationStack)[i] = s;
                setStation(s);
                return 1;
            }
        }
        return 0;
    }
    if ((*stationStack)[2] != 0) {
        delete (*stationStack)[2];
    }
    (*stationStack)[2] = 0;
    for (int i = 2; i != 0; i = i - 1) {
        (*stationStack)[i] = (*stationStack)[i - 1];
    }
    (*stationStack)[0] = s;
    setStation(s);
    return 1;
}

void Status::removeMission(Mission *mission) {
    for (unsigned i = 0; i < missions->size(); i = i + 1) {
        if ((*missions)[i] == mission) {
            if (this->mission == mission) {
                mission = 0;
                this->mission = 0;
            }
            if ((*missions)[i]->getAgent() != 0) {
                (*missions)[i]->getAgent()->setMission(0);
            }
            if ((*missions)[i] != 0) {
                delete (*missions)[i];
            }
            (*missions)[i] = 0;
            (*missions)[i] = Mission::empty;
        }
    }
}

// Campaign missions whose completion awards the medal flag at +0x17c.
static const int kMedalCampaignMissions[3] = {93, 111, 143};

void Status::nextCampaignMission(bool advance) {
    int mission = this->currentCampaignMission;
    this->currentCampaignMission = mission + 1;
    for (int i = 0; i < 3; i++) {
        if (this->currentCampaignMission == kMedalCampaignMissions[i]) {
            this->field_0x17c = 1;
        }
    }
    reinterpret_cast<int64_t &>(this->field_0x100) = this->playingTime;

    switch (mission) {
        case 52:
        case 128:
            nextCampaignMission(advance);
            break;
        case 147:
        case 148:
        case 149:
        case 150:
            setCampaignMission(new Mission(171, 0, 96));
            break;
        case 44: {
            this->field_7c = -10;
            this->field_80 = -10;
            setCampaignMission(new Mission());
            (*this->missions)[0]->setVisible(false);
            int credits = this->credits + 40000;
            if (credits < 0) {
                credits = 0;
            }
            this->credits = credits;
            break;
        }
        case 21:
            setCampaignMission(new Mission(11, 0, 55));
            break;
        case 7: {
            Array<Item *> *equipment = this->ship->getEquipment();
            if (equipment != 0) {
                for (unsigned i = 0; i < equipment->size(); i++) {
                    if ((*equipment)[i] != 0) {
                        (*equipment)[i]->setUnsaleable(false);
                        (*equipment)[i]->setPrice((*Globals::items)[i]->getSinglePrice());
                    }
                }
            }
            Array<Item *> *cargo = this->ship->getCargo();
            if (cargo != 0) {
                for (unsigned i = 0; i < cargo->size(); i++) {
                    if ((*cargo)[i] != 0) {
                        (*cargo)[i]->setUnsaleable(false);
                        (*cargo)[i]->setPrice((*Globals::items)[i]->getSinglePrice());
                    }
                }
            }
            setCampaignMission(new Mission(11, 0, 78));
            break;
        }
        case 38:
            setCampaignMission(new Mission(11, 0, 30));
            break;
        case 11:
            setCampaignMission(new Mission(11, 0, 79));
            break;
        case 47: {
                this->field_8c = (int32_t)(intptr_t)this->ship;
                this->ship = 0;
                setShip((*Globals::ships)[9]->makeShip(-1));
                this->ship->setRace(1);
                Item *item = (*Globals::items)[58]->makeItem();
                Globals::status->ship->setEquipment(item, 0);
                item = (*Globals::items)[83]->makeItem();
                Globals::status->ship->setEquipment(item, 1);
                Globals::status->fadeValue = 0;
                setCampaignMission(new Mission(11, 0, 58));
                break;
            }
        case 16:
            setCampaignMission(new Mission(11, 0, 98));
            break;
        case 24: {
            Array<Item *> *cargo = this->ship->getCargo();
            if (cargo != 0) {
                for (unsigned i = 0; i < cargo->size(); i++) {
                    if ((*cargo)[i]->getIndex() == 131) {
                        (*cargo)[i]->setUnsaleable(true);
                        break;
                    }
                }
            }
            setCampaignMission(new Mission(156, 0, -1));
            break;
        }
        case 14:
            setCampaignMission(new Mission(11, 0, 98));
            break;
        case 18:
            setCampaignMission(new Mission(156, 0, 55));
            break;
        case 42:
            setCampaignMission(new Mission(11, 0, 10));
            break;
        case 45:
        case 106:
            break;
        case 49:
            setCampaignMission(new Mission(156, 0, 62));
            break;
        case 8:
            setCampaignMission(new Mission(11, 0, 78));
            break;
        case 31:
            setCampaignMission(new Mission(11, 0, 10));
            break;
        case 39:
            setCampaignMission(new Mission(161, 0, -1));
            break;
        case 30:
            setCampaignMission(new Mission(11, 30000, 98));
            break;
        case 32:
            setCampaignMission(new Mission(8, 0, 10));
            (*this->missions)[0]->setProductionGoods(164, 50);
            break;
        case 33:
            this->ship->removeCargo(164, 50);
            for (unsigned i = 0; i < this->bluePrints->size(); i++) {
                if ((*this->bluePrints)[i]->getIndex() == 85) {
                    (*this->bluePrints)[i]->unlock();
                    (*this->bluePrints)[i]->addItem((*Globals::items)[164]->makeItem(), 50, 10);
                    break;
                }
            }
            setCampaignMission(new Mission(11, 0, 30));
            break;
        case 29:
            setCampaignMission(new Mission(156, 0, 91));
            break;
        case 1:
            setCampaignMission(new Mission(154, 0, 78));
            (*this->missions)[0]->setStatusValue(10);
            break;
        case 20:
            setCampaignMission(new Mission(4, 0, 55));
            break;
        case 23:
            this->field_80 = 48;
            this->field_7c = 9;
            setCampaignMission(new Mission(4, 0, 48));
            break;
        case 34:
            setCampaignMission(new Mission(11, 0, 29));
            break;
        case 51:
            setCampaignMission(new Mission(160, 0, 25));
            (*this->missions)[0]->setVisible(false);
            break;
        case 53:
            setCampaignMission(new Mission(11, 200000, 74));
            break;
        case 58:
            if (Globals::status->field_90 != 0) {
                delete Globals::status->field_90;
            }
            Globals::status->field_90 = 0;
            Globals::status->field_90 = new Array<int>();
            ArraySetLength(3, *Globals::status->field_90);
            (*Globals::status->field_90)[0] = 56;
            (*Globals::status->field_90)[1] = 45;
            (*Globals::status->field_90)[2] = 22;
            for (unsigned i = 0; i < this->bluePrints->size(); i++) {
                if ((*this->bluePrints)[i]->getIndex() == 179) {
                    (*this->bluePrints)[i]->lock();
                }
            }
            setCampaignMission(new Mission(163, 0, 101));
            (*this->missions)[0]->setStatusValue(0);
            Globals::status->field_10c = 0;
            break;
        case 59: {
            int statusValue = (*this->missions)[0]->getStatusValue();
            setCampaignMission(new Mission(11, (statusValue + 1) * 50000, 101));
            (*this->missions)[0]->setStatusValue(statusValue > 0);
            break;
        }
        case 73:
            setCampaignMission(new Mission(11, 0, 100));
            break;
        case 77: {
            Item *item = Globals::status->ship->getFirstEquipmentOfSort(18);
            if (item != 0) {
                Globals::status->ship->removeEquipment(item);
            } else {
                item = Globals::status->ship->getCargo(85);
                if (item != 0) {
                    Globals::status->ship->removeCargo(item);
                }
            }
            for (unsigned i = 0; i < this->bluePrints->size(); i++) {
                if (!(*this->bluePrints)[i]->isEmpty() && (*this->bluePrints)[i]->getStationIndex() == 101) {
                    (*this->bluePrints)[i]->reset();
                }
            }
            setCampaignMission(new Mission(4, 0, 101));
            break;
        }
        case 80:
            setCampaignMission(new Mission(4, 0, -1));
            break;
        case 87:
            setCampaignMission(new Mission(11, 0, 10));
            break;
        case 99:
            setCampaignMission(new Mission(4, 0, 98));
            break;
        case 109:
            setCampaignMission(new Mission(171, 0, 10));
            break;
        case 113:
            setCampaignMission(new Mission(4, 0, 83));
            break;
        case 157:
            setCampaignMission(new Mission(4, 0, 111));
            break;
        case 159:
            setCampaignMission(new Mission(170, 0, 10));
            break;
        case 0:
            setCampaignMission(new Mission(11, 0, 78));
            this->mission = (*this->missions)[0];
            break;
        case 2:
            setCampaignMission(new Mission(11, 0, 78));
            break;
        case 12:
            setCampaignMission(new Mission(150, 0, 0));
            (*this->missions)[0]->setStatusValue(this->missionCount + 1);
            (*this->missions)[0]->setVisible(false);
            break;
        case 28:
            setCampaignMission(new Mission(4, 0, -1));
            break;
        case 54:
            (*this->systemVisibilities)[23] = 1;
            setCampaignMission(new Mission(11, 0, 101));
            setShip((Ship *)(intptr_t)this->field_8c);
            this->field_8c = 0;
            break;
        case 55: {
                this->field_8c = (int32_t)(intptr_t)this->ship;
                this->ship = 0;
                setShip((*Globals::ships)[39]->makeShip(-1));
                this->ship->setRace(1);
                Item *item = (*Globals::items)[181]->makeItem();
                Globals::status->ship->setEquipment(item, 0);
                item = (*Globals::items)[52]->makeItem();
                Globals::status->ship->setEquipment(item, 0);
                item = (*Globals::items)[58]->makeItem();
                Globals::status->ship->setEquipment(item, 1);
                item = (*Globals::items)[83]->makeItem();
                Globals::status->ship->setEquipment(item, 2);
                setCampaignMission(new Mission(4, 0, 102));
                break;
            }
        case 56:
            setCampaignMission(new Mission(11, 150000, 101));
            break;
        case 57:
            setCampaignMission(new Mission(166, 0, 101));
            (*this->missions)[0]->setProductionGoods(179, 10);
            for (unsigned i = 0; i < this->bluePrints->size(); i++) {
                if ((*this->bluePrints)[i]->getIndex() == 179) {
                    (*this->bluePrints)[i]->unlock();
                    (*this->bluePrints)[i]->addItem((*Globals::items)[127], 5, 101);
                    break;
                }
            }
            setShip((Ship *)(intptr_t)this->field_8c);
            this->field_8c = 0;
            break;
        case 60:
            setCampaignMission(new Mission(164, 0, 101));
            break;
        case 61:
            (*this->systemVisibilities)[22] = 1;
            setCampaignMission(new Mission(11, 0, 100));
            break;
        case 62:
            (*this->systemVisibilities)[24] = 1;
            setCampaignMission(new Mission(4, 0, 103));
            break;
        case 63:
            setCampaignMission(new Mission(4, 0, 104));
            break;
        case 64:
            setCampaignMission(new Mission(11, 0, 100));
            break;
        case 65:
            setCampaignMission(new Mission(11, 0, 101));
            break;
        case 66:
            setCampaignMission(new Mission(4, 0, 104));
            (*this->missions)[0]->setStatusValue(0);
            Globals::status->station->addItem((*Globals::items)[60]->makeItem(5, 0));
            Globals::status->station->addItem((*Globals::items)[61]->makeItem(5, 0));
            Globals::status->station->addItem((*Globals::items)[62]->makeItem(5, 0));
            break;
        case 67:
            setCampaignMission(new Mission(8, 0, 66));
            (*this->missions)[0]->setProductionGoods(175, 1);
            break;
        case 68:
            Globals::status->ship->removeCargo(175);
            setCampaignMission(new Mission(6, 0, 66));
            break;
        case 69:
            setCampaignMission(new Mission(6, 0, 65));
            break;
        case 70:
            setCampaignMission(new Mission(11, 0, 66));
            break;
        case 71:
            for (unsigned i = 0; i < this->bluePrints->size(); i++) {
                if ((*this->bluePrints)[i]->getIndex() == 183) {
                    (*this->bluePrints)[i]->unlock();
                }
            }
            Globals::status->ship->addCargo((*Globals::items)[175]->makeItem(1));
            setCampaignMission(new Mission(164, 150000, 101));
            this->field_10c = 0;
            break;
        case 72:
            setCampaignMission(new Mission(10, 0, 81));
            break;
        case 74:
            Globals::status->station->removeShips();
            setCampaignMission(new Mission(11, 0, 100));
            break;
        case 75:
            setCampaignMission(new Mission(11, 0, 100));
            break;
        case 76: {
            Item *item = Globals::status->ship->getFirstEquipmentOfSort(18);
            if (item != 0) {
                item->setUnsaleable(true);
            }
            setCampaignMission(new Mission(11, 0, 101));
            break;
        }
        case 78:
            setCampaignMission(new Mission(165, 0, -1));
            break;
        case 79:
            setCampaignMission(new Mission(1, 0, 100));
            break;
        case 81:
            setCampaignMission(new Mission(11, 0, 100));
            break;
        case 82:
            setCampaignMission(new Mission(11, 0, 100));
            break;
        case 83: {
            Item *item = Globals::status->ship->getFirstEquipmentOfSort(18);
            if (item == 0) {
                item = Globals::status->ship->getCargo(85);
            }
            if (item != 0) {
                item->setUnsaleable(false);
            }
            Globals::status->ship->addCargo((*Globals::items)[85]->makeItem(1));
            setCampaignMission(new Mission());
            (*this->missions)[0]->setVisible(false);
            break;
        }
        case 84:
            setCampaignMission(new Mission(164, 0, 0));
            break;
        case 85:
            setCampaignMission(new Mission(11, 0, 100));
            break;
        case 86:
            setCampaignMission(new Mission(4, 0, 10));
            break;
        case 88:
            Globals::status->ship->removeCargo(104, 10);
            (*Globals::status->systemVisibilities)[27] = 1;
            (*Globals::status->systemVisibilities)[28] = 1;
            setCampaignMission(new Mission(4, 0, 109));
            break;
        case 89:
            Globals::status->stationsVisited--;
            Globals::galaxy->getVisited()[109] = false;
            setCampaignMission(new Mission(11, 0, 10));
            break;
        case 90:
            (*this->systemVisibilities)[27] = 1;
            (*this->systemVisibilities)[28] = 1;
            setCampaignMission(new Mission(184, 0, 110));
            (*this->missions)[0]->setProductionGoods(0, 10);
            (*this->missions)[0]->setStatusValue(10);
            this->field_178 = 0;
            break;
        case 91:
            setCampaignMission(new Mission(184, 0, 113));
            (*this->missions)[0]->setProductionGoods(0, 10);
            (*this->missions)[0]->setStatusValue(10);
            break;
        case 92:
            setCampaignMission(new Mission(11, 0, 114));
            break;
        case 93: {
                Item *item = (*Globals::items)[205]->makeItem(1);
                Globals::status->ship->addCargo(item);
                setCampaignMission(new Mission(184, 0, 111));
                (*this->missions)[0]->setProductionGoods(0, 83);
                (*this->missions)[0]->setStatusValue(83);
                this->field_178 = 0;
                break;
            }
        case 94:
            setCampaignMission(new Mission(170, 0, 10));
            (*this->missions)[0]->setStatusValue(0);
            break;
        case 95:
            setCampaignMission(new Mission(11, 0, 98));
            break;
        case 96:
            setCampaignMission(new Mission(4, 0, 85));
            break;
        case 97:
            (*Globals::status->systemVisibilities)[29] = 1;
            setCampaignMission(new Mission(11, 0, 120));
            break;
        case 98:
            setCampaignMission(new Mission(170, 0, 10));
            (*this->missions)[0]->setStatusValue(0);
            break;
        case 100:
            setCampaignMission(new Mission(11, 0, 98));
            break;
        case 101: {
                Item *item = (*Globals::items)[207]->makeItem(1);
                Globals::status->ship->addCargo(item);
                setCampaignMission(new Mission(184, 0, 113));
                (*this->missions)[0]->setProductionGoods(0, 1700);
                (*this->missions)[0]->setStatusValue(1700);
                break;
            }
        case 102:
            setCampaignMission(new Mission(11, 0, 10));
            break;
        case 103:
            for (unsigned i = 0; i < this->bluePrints->size(); i++) {
                if ((*this->bluePrints)[i]->getIndex() == 206) {
                    (*this->bluePrints)[i]->unlock();
                    (*this->bluePrints)[i]->addItem((*Globals::items)[163]->makeItem(), 10, 10);
                    break;
                }
            }
            setCampaignMission(new Mission(166, 0, 10));
            (*this->missions)[0]->setProductionGoods(206, 1);
            break;
        case 104:
            setCampaignMission(new Mission(4, 0, 109));
            break;
        case 105:
            setCampaignMission(new Mission(4, 0, 111));
            break;
        case 107:
            setCampaignMission(new Mission(11, 0, 10));
            break;
        case 108:
            setCampaignMission(new Mission(170, 0, 114));
            (*this->missions)[0]->setStatusValue(0);
            break;
        case 110:
            setCampaignMission(new Mission(171, 0, 38));
            break;
        case 111:
            setCampaignMission(new Mission(172, 0, 38));
            (*this->missions)[0]->setProductionGoods(146, 1);
            break;
        case 112:
            Globals::status->ship->removeCargo(146, 1);
            setCampaignMission(new Mission(171, 0, 82));
            break;
        case 114:
            setCampaignMission(new Mission(171, 0, 82));
            break;
        case 115:
            setCampaignMission(new Mission(171, 0, 93));
            (*this->missions)[0]->setStatusValue(0);
            break;
        case 116:
            (*this->systemVisibilities)[30] = 1;
            setCampaignMission(new Mission(11, 0, 126));
            break;
        case 117:
            setCampaignMission(new Mission(8, 0, 126));
            (*this->missions)[0]->setProductionGoods(209, 1);
            break;
        case 118: {
            Array<Item *> *cargo = this->ship->getCargo();
            if (cargo != 0) {
                for (unsigned i = 0; i < cargo->size(); i++) {
                    if ((*cargo)[i]->getIndex() == 209) {
                        (*cargo)[i]->setUnsaleable(true);
                        break;
                    }
                }
            }
            setCampaignMission(new Mission(170, 0, 10));
            (*this->missions)[0]->setStatusValue(0);
            break;
        }
        case 119:
            setCampaignMission(new Mission(4, 0, 40));
            break;
        case 120:
            setCampaignMission(new Mission(8, 0, 93));
            (*this->missions)[0]->setProductionGoods(209, 1);
            break;
        case 121: {
            Array<Item *> *cargo = this->ship->getCargo();
            if (cargo != 0) {
                for (unsigned i = 0; i < cargo->size(); i++) {
                    if ((*cargo)[i]->getIndex() == 209) {
                        (*cargo)[i]->setUnsaleable(false);
                        break;
                    }
                }
            }
            Globals::status->ship->removeCargo(209, 1);
            setCampaignMission(new Mission(11, 0, 10));
            break;
        }
        case 122:
            setCampaignMission(new Mission(4, 0, 121));
            break;
        case 123:
            setCampaignMission(new Mission(11, 0, 121));
            break;
        case 124:
            setCampaignMission(new Mission(4, 0, 55));
            break;
        case 125:
            setCampaignMission(new Mission(170, 0, 120));
            (*this->missions)[0]->setStatusValue(0);
            break;
        case 126:
            setCampaignMission(new Mission(11, 0, 98));
            break;
        case 127:
            setCampaignMission(new Mission());
            (*this->missions)[0]->setVisible(false);
            break;
        case 129:
            setCampaignMission(new Mission());
            (*this->missions)[0]->setVisible(false);
            break;
        case 130:
            setCampaignMission(new Mission(4, 0, 112));
            break;
        case 131:
            setCampaignMission(new Mission(11, 0, 112));
            break;
        case 132:
            setCampaignMission(new Mission(170, 0, 120));
            (*this->missions)[0]->setStatusValue(0);
            break;
        case 133:
            setCampaignMission(new Mission(11, 0, 22));
            (*this->missions)[0]->setStatusValue(0);
            break;
        case 134:
            setCampaignMission(new Mission(174, 0, 103));
            (*this->missions)[0]->setProductionGoods(155, 140);
            (*this->missions)[0]->setStatusValue(0);
            break;
        case 135:
            setCampaignMission(new Mission(11, 0, 112));
            break;
        case 136:
            setCampaignMission(new Mission(4, 0, 58));
            break;
        case 137:
            setCampaignMission(new Mission(11, 0, 58));
            break;
        case 138:
            (*this->systemVisibilities)[31] = 1;
            setCampaignMission(new Mission(168, 0, 131));
            (*this->missions)[0]->setStatusValue(10);
            this->field_178 = 0;
            break;
        case 139:
            setCampaignMission(new Mission(11, 0, 112));
            break;
        case 140:
            for (unsigned i = 0; i < this->bluePrints->size(); i++) {
                if ((*this->bluePrints)[i]->getIndex() == 210) {
                    (*this->bluePrints)[i]->unlock();
                    (*this->bluePrints)[i]->addItem((*Globals::items)[201]->makeItem(), 847, 112);
                    (*this->bluePrints)[i]->addItem((*Globals::items)[202]->makeItem(), 834, 112);
                    (*this->bluePrints)[i]->addItem((*Globals::items)[203]->makeItem(), 861, 112);
                    (*this->bluePrints)[i]->addItem((*Globals::items)[204]->makeItem(), 892, 112);
                    break;
                }
            }
            setCampaignMission(new Mission(11, 0, 78));
            break;
        case 141:
            Globals::status->ship->addCargo((*Globals::items)[197]->makeItem(15));
            Globals::status->ship->addCargo((*Globals::items)[196]->makeItem(1));
            Globals::status->ship->addCargo((*Globals::items)[198]->makeItem(1));
            setCampaignMission(new Mission(4, 0, 79));
            break;
        case 142:
            setCampaignMission(new Mission(8, 0, 112));
            (*this->missions)[0]->setProductionGoods(210, 1);
            break;
        case 143:
            Globals::status->ship->removeCargo(210, 1);
            setCampaignMission(new Mission(170, 0, 112));
            (*this->missions)[0]->setStatusValue(0);
            break;
        case 144:
            setCampaignMission(new Mission(4, 0, 112));
            break;
        case 145:
            setCampaignMission(new Mission(11, 0, 112));
            break;
        case 146:
            setCampaignMission(new Mission(4, 0, -1));
            break;
        case 151:
            setCampaignMission(new Mission(165, 0, -1));
            break;
        case 152:
            setCampaignMission(new Mission(11, 0, 98));
            break;
        case 153:
            setCampaignMission(new Mission(4, 0, -1));
            break;
        case 154:
            setCampaignMission(new Mission(164, 0, 0));
            break;
        case 155:
            setCampaignMission(new Mission(11, 0, 99));
            break;
        case 156:
            setCampaignMission(new Mission(4, 0, 112));
            break;
        case 158:
            setCampaignMission(new Mission(11, 0, 10));
            break;
        case 160:
            setCampaignMission(new Mission(170, 0, 93));
            (*this->missions)[0]->setStatusValue(0);
            break;
        case 161:
            setCampaignMission(new Mission());
            (*this->missions)[0]->setVisible(false);
            break;
        case 3:
            this->ship->setCargo(0);
            setCampaignMission(new Mission(154, 0, 78));
            (*this->missions)[0]->setStatusValue(25);
            break;
        case 9: {
            Item *engine = this->ship->getFirstEquipmentOfSort(19);
            this->ship->removeEquipment(engine);
            this->ship->addEquipment((*Globals::items)[86]->makeItem(1));
            setCampaignMission(new Mission(11, 0, 79));
            break;
        }
        case 35:
            setCampaignMission(new Mission(12, 0, 27));
            break;
        case 40:
            setCampaignMission(new Mission(4, 0, -1));
            break;
        case 43:
            setCampaignMission(new Mission(11, 0, 10));
            break;
        case 4:
            setCampaignMission(new Mission(11, 0, 78));
            break;
        case 27:
            this->field_80 = 91;
            this->field_7c = 18;
            setCampaignMission(new Mission(4, 0, 91));
            break;
        case 46:
            setCampaignMission(new Mission(11, 0, 74));
            break;
        case 17:
            this->ship->setRace(0);
            setCampaignMission(new Mission(156, 0, 56));
            break;
        case 36:
            setCampaignMission(new Mission(160, 0, 27));
            (*this->missions)[0]->setVisible(false);
            break;
        case 48: {
                setShip((*Globals::ships)[41]->makeShip(-1));
                this->ship->setRace(1);
                for (int i = 0; i < 3; i++) {
                    Item *item = (*Globals::items)[177]->makeItem();
                    Globals::status->ship->setEquipment(item, i);
                }
                Item *item = (*Globals::items)[58]->makeItem();
                Globals::status->ship->setEquipment(item, 0);
                item = (*Globals::items)[83]->makeItem();
                Globals::status->ship->setEquipment(item, 1);
                item = (*Globals::items)[52]->makeItem();
                Globals::status->ship->setEquipment(item, 2);
                setCampaignMission(new Mission(156, 0, 58));
                break;
            }
        case 5:
            this->ship->removeAllCargo();
            setCampaignMission(new Mission(158, 0, 78));
            break;
        case 15:
            setCampaignMission(new Mission(4, 0, 98));
            break;
        case 22:
            (*this->systemVisibilities)[6] = 1;
            setCampaignMission(new Mission(11, 20000, 10));
            break;
        case 10:
            setCampaignMission(new Mission(11, 0, 76));
            break;
        case 19:
            setCampaignMission(new Mission(189, 0, 55));
            (*this->missions)[0]->setStatusValue(6);
            break;
        case 26:
            setCampaignMission(new Mission(11, 0, 10));
            break;
        case 13:
            setCampaignMission(new Mission(4, 0, 79));
            break;
        case 37:
            setCampaignMission(new Mission(4, 0, 22));
            break;
        case 50:
            setCampaignMission(new Mission(156, 0, 25));
            break;
        case 6:
            setCampaignMission(new Mission(4, 0, 78));
            break;
        case 41:
            this->field_7c = -10;
            this->field_80 = -10;
            setCampaignMission(new Mission(160, 0, -1));
            break;
        case 25:
            this->field_7c = -1;
            this->field_80 = -1;
            setCampaignMission(new Mission(4, 0, 48));
            break;
        default:
            break;
    }
}

void Status::setStation(Station *s) {
    if (this->station == s) {
        return;
    }
    this->station = s;
    this->system = Globals::galaxy->getSystem(s->getSystem());
    if (this->system == 0) {
        return;
    }
    (*this->field_b4)[this->station->getSystem()] = true;

    FileRead *fileRead = new FileRead();
    Array<Station *> *stationList = fileRead->loadStationsBinary(asSystem(this->system));
    delete fileRead;

    Array<String *> *names = (Array<String *> *) (intptr_t) this->planetNames;
    if (names != 0) {
        ArrayReleaseClasses(*names);
        if ((Array<String *> *) (intptr_t) this->planetNames != 0) {
            delete (Array<String *> *) (intptr_t) this->planetNames;
        }
    }
    this->planetNames = 0;
    names = new Array<String *>();
    this->planetNames = (int32_t) (intptr_t) names;
    ArraySetLength(stationList->size(), *names);

    if ((Array<int> *) (intptr_t) this->planetTextures != 0) {
        ArrayRelease(*(Array<int> *) (intptr_t) this->planetTextures);
        if ((Array<int> *) (intptr_t) this->planetTextures != 0) {
            delete (Array<int> *) (intptr_t) this->planetTextures;
        }
    }
    this->planetTextures = 0;
    Array<int> *textures = new Array<int>();
    this->planetTextures = (int32_t) (intptr_t) textures;
    ArraySetLength(stationList->size(), *textures);

    Array<int> *stationIndices = (Array<int> *) asSystem(this->system)->getStations();
    __atomic_signal_fence(__ATOMIC_SEQ_CST);
    for (unsigned i = 0; i < ((Array<int> *) asSystem(this->system)->getStations())->size(); i++) {
        for (unsigned j = 0; j < stationList->size(); j++) {
            Station *candidate = (*stationList)[j];
            if ((*stationIndices)[i] == candidate->getIndex()) {
                String *name;
                if (this->currentCampaignMission != 0) {
                    name = new String(candidate->getName());
                } else {
                    name = new String();
                }
                (*(Array<String *> *) (intptr_t) this->planetNames)[i] = name;
                (*(Array<int> *) (intptr_t) this->planetTextures)[i] = candidate->getTextureIndex();
                break;
            }
        }
    }
    ArrayReleaseClasses(*stationList);
    delete stationList;
}

String Status::replaceHash(String text, String replacement, String hash) {
    int index = (int) text.IndexOf(hash);
    if (index >= 0) {
        String prefix = text.SubString(0, index);
        if (prefix.length != 0) {
            return prefix + String(replacement) + text.SubString(hash.length + index, text.length);
        }
        return String(replacement) + text.SubString(hash.length + index, text.length);
    }
    return text;
}

String Status::replaceHash(String text, String replacement) {
    return replaceHash(text, replacement, String("#"));
}

void Status::changeRating(int delta) {
    int updated = delta + rating;
    rating = updated;
    if (updated > 10) {
        rating = 10;
    } else {
        if (-0xb < updated) {
            return;
        }
        rating = -10;
    }
}


void Status::departStation(Station *dest) {
    if (!inAlienOrbit() && station->getIndex() == 108 && field_114 == 3) {
        voidStation->setItems(station->getItems(), true);
        voidStation->setShips(station->getShips(), true);
    }
    bool moveWantedTargets = false;
    if (!inAlienOrbit() && playerStation != dest) {
        moveWantedTargets = dest->getIndex() != station->getIndex();
    }
    if (playerStation != dest) {
        Station *stacked = isOnStack(dest);
        addStationToStack(dest);
        if (dest->getIndex() == 108 && field_114 == 3) {
            dest->setItems(voidStation->getItems(), true);
            dest->setShips(voidStation->getShips(), true);
            if (stacked != 0 && stacked != dest) {
                stacked->setItems(voidStation->getItems(), true);
                stacked->setShips(voidStation->getShips(), true);
            }
            if (stacked == 0) {
                Generator *generator = new Generator();
                dest->setAgents(generator->createAgents(dest));
            }
        } else if (stacked == 0) {
            Generator *generator = new Generator();
            dest->setItems(generator->getItemBuyList(dest), false);
            dest->setShips(generator->getShipBuyList(dest), false);
            dest->setAgents(generator->createAgents(dest));
        }
        field_70 = (int32_t) playingTime;
        field_74 = (int32_t) (playingTime >> 32);
    }

    this->mission = Mission::empty;
    for (unsigned i = 0; i < missions->size(); i++) {
        if ((*missions)[i] == 0) {
            continue;
        }
        int type = (*missions)[i]->getType();
        if ((*missions)[i]->isCampaignMission() && type == 0xad &&
            Globals::status->ship->hasCargo((*missions)[i]->getProductionGoodIndex(), 1)) {
            this->mission = (*missions)[i];
            break;
        }
        if (Globals::status->currentCampaignMission < 45 && (*missions)[i]->isCampaignMission() && type == 0xa1 &&
            dest->getIndex() == field_80 && !Globals::status->inAlienOrbit()) {
            this->mission = (*missions)[i];
            break;
        }
        if (!(*missions)[i]->isEmpty() && (*missions)[i]->getTargetStation() == dest->getIndex()) {
            bool deliveryType = type != 0x96 && type != 0x97 && type != 0x99 && type != 0x9b && type != 0xa1 &&
                                type != 0xa2 && type != 0xa3 && type != 0xa4 && type != 0xa6 && type != 0xa7 &&
                                type != 0xa9 && type != 0xad && type != 8 && type != 0xe;
            if (deliveryType) {
                if ((*missions)[i]->isCampaignMission()) {
                    this->mission = (*missions)[i];
                    break;
                }
                if (type == 0xb || type == 0xd) {
                    break;
                }
                this->mission = (*missions)[i];
                break;
            }
        }
        if (!(*missions)[i]->isEmpty() && (*missions)[i]->isCampaignMission()) {
            if ((*missions)[i]->getTargetStation() != dest->getIndex() && type == 0xa0) {
                this->mission = (*missions)[i];
                break;
            }
        }
    }

    if (currentCampaignMission < 45) {
        if (currentCampaignMission >= 32 && dest->getIndex() != (*missions)[0]->getTargetStation() &&
            dest->getIndex() != field_80) {
            if (field_88++ >= 10) {
                field_88 = 0;
                int system;
                do {
                    do {
                        do {
                            system = Globals::rnd->nextInt(22);
                            field_7c = system;
                        } while (!(*systemVisibilities)[system]);
                    } while (system == 10 || system == 15);
                } while (this->mission->isCampaignMission() && this->mission->getTargetStation() == field_80);
                SolarSystem *solarSystem = asSystem(Globals::galaxy->getSystem(field_7c));
                Array<int> *stations = reinterpret_cast<Array<int> *>(solarSystem->getStations());
                field_80 = (*stations)[Globals::rnd->nextInt(
                    reinterpret_cast<Array<int> *>(solarSystem->getStations())->size())];
            }
        }
    } else {
        field_7c = -10;
        field_80 = -10;
    }

    if (moveWantedTargets) {
        moveWanted();
    }
    Array<Item *> *cargo = ship->getCargo();
    if (cargo != 0) {
        for (unsigned i = 0; i < cargo->size(); i++) {
            if ((*cargo)[i] != 0) {
                (*cargo)[i]->setStationAmount(0);
            }
        }
    }
    field_0x108 = 0;
    ship->refreshValue();
}

Mission *Status::missionCompleted(bool atStation, bool docked, long long extra) {
    for (unsigned i = 0; i < missions->size(); i++) {
        Mission *m = (*missions)[i];
        if (m->hasWon()) {
            return 0;
        }
        if (m == 0) {
            continue;
        }
        // The original queries the target station twice here.
        if (!m->isCampaignMission() && m->getAgent() == 0 && m->getClientImage() == 0 &&
            m->getTargetStation() == 0 && m->getTargetStation() == 0) {
            continue;
        }
        switch (m->getType()) {
            case 0xa7:
            case 0xae:
                if (m->getStatusValue() >= m->getProductionGoodAmount()) {
                    m->setWon(true);
                    return m;
                }
                break;
            case 8:
                if (m->isCampaignMission() && Globals::status->currentCampaignMission == 0x8f && !atStation) {
                    if (station->getIndex() == m->getTargetStation() && pendingProducts != 0) {
                        for (unsigned j = 0; j < pendingProducts->size(); j++) {
                            PendingProduct *pp = (*pendingProducts)[j];
                            if (pp != 0 && pp->blueprintIndex == 0xd2) {
                                return m;
                            }
                        }
                    }
                }
                if (atStation && station->getIndex() == m->getTargetStation()) {
                    int index = m->getProductionGoodIndex();
                    int amount = m->getProductionGoodAmount();
                    if (Item::isInList(index, amount, ship->getCargo())) {
                        return m;
                    }
                }
                break;
            case 0:
            case 0xb:
                if (atStation && station->getIndex() == m->getTargetStation()) {
                    return m;
                }
                break;
            case 0xd:
                if (atStation && field_0xf0) {
                    return m;
                }
                break;
            case 0xe:
                if (atStation && station->getIndex() == m->getAgent()->getStation() &&
                    Item::isInList(0x73, ship->getCargo())) {
                    return m;
                }
                break;
            case 0x96:
                if (missionCount >= m->getStatusValue()) {
                    m->setWon(true);
                    return m;
                }
                break;
            case 0x97:
                if (kills >= m->getStatusValue()) {
                    m->setWon(true);
                    return m;
                }
                break;
            case 0x98: {
                Array<Item *> *equipment = ship->getEquipment();
                for (unsigned j = 0; j < equipment->size(); j++) {
                    if ((*equipment)[j] != 0 && (*equipment)[j]->getIndex() == m->getStatusValue()) {
                        return m;
                    }
                }
                break;
            }
            case 0x99:
                if (stationsVisited >= m->getStatusValue()) {
                    m->setWon(true);
                    return m;
                }
                break;
            case 0x9a:
                if (ship->getCurrentLoad() >= m->getStatusValue()) {
                    m->setWon(true);
                    return m;
                }
                break;
            case 0x9b:
                if (goodsProduced >= m->getStatusValue()) {
                    m->setWon(true);
                    return m;
                }
                break;
            case 0x9e: {
                Array<Item *> *equipment = ship->getEquipment();
                bool hasSpecial = false;
                bool hasGood = false;
                for (unsigned j = 0; j < equipment->size(); j++) {
                    if ((*equipment)[j] != 0 && (*equipment)[j]->getType() == 0) {
                        hasGood = true;
                    } else if ((*equipment)[j] != 0 && (*equipment)[j]->getSort() == 10) {
                        hasSpecial = true;
                    }
                }
                if (hasGood && hasSpecial) {
                    return m;
                }
                break;
            }
            case 0xa2:
                if (atStation && station->getIndex() == m->getTargetStation()) {
                    BluePrint *bluePrint = (*bluePrints)[m->getStatusValue()];
                    Array<int> *ingredients = bluePrint->getIngredientList();
                    Array<int> *quantities = bluePrint->getQuantityList();
                    bool complete = true;
                    for (unsigned j = 0; j < ingredients->size(); j++) {
                        if (!ship->hasCargo((*ingredients)[j], (*quantities)[j])) {
                            complete = false;
                            break;
                        }
                    }
                    if (complete) {
                        return m;
                    }
                }
                break;
            case 0xa6:
                if (atStation && station->getIndex() == m->getTargetStation()) {
                    int index = m->getProductionGoodIndex();
                    int amount = m->getProductionGoodAmount();
                    if (Item::isInList(index, amount, ship->getCargo())) {
                        return m;
                    }
                    if (Globals::status->ship->hasEquipment(m->getProductionGoodIndex(), 1)) {
                        return m;
                    }
                }
                break;
            case 0xaa:
                if (m->getStatusValue() == 1) {
                    m->setWon(true);
                    return m;
                }
                break;
            case 0xb8:
                if (m->getStatusValue() == 0) {
                    if (Globals::status->currentCampaignMission != 0x5c || Globals::status->inAlienOrbit() ||
                        Globals::status->station->getIndex() != 0x71) {
                        m->setWon(true);
                        return m;
                    }
                }
                break;
            case 0xbd:
                if (atStation) {
                    Array<Item *> *equipment = ship->getEquipment();
                    for (unsigned j = 0; j < equipment->size(); j++) {
                        if ((*equipment)[j] != 0 && (*equipment)[j]->getSort() == m->getStatusValue()) {
                            return m;
                        }
                    }
                }
                break;
            case 0x9c:
                if (!atStation && station->getIndex() == m->getTargetStation() && extra > 10000) {
                    return m;
                }
                break;
            case 0x9d: {
                Array<Item *> *equipment = ship->getEquipment();
                for (unsigned j = 0; j < equipment->size(); j++) {
                    if ((*equipment)[j] != 0 && (*equipment)[j]->getType() == m->getStatusValue()) {
                        return m;
                    }
                }
                break;
            }
            case 0xa0:
                if (atStation) {
                    return m;
                }
                if (extra > 10000 && station->getIndex() != m->getTargetStation()) {
                    return m;
                }
                break;
            case 0xa3: {
                bool none = true;
                for (unsigned j = 0; j < field_90->size(); j++) {
                    if ((*field_90)[j] >= 0) {
                        none = false;
                        break;
                    }
                }
                if (none) {
                    return m;
                }
                break;
            }
            case 0xa4:
                if (!atStation && extra > 10000) {
                    return m;
                }
                break;
            case 0xa5:
                return m;
            case 0xa8:
                if (Globals::status->field_178 >= m->getStatusValue()) {
                    m->setWon(true);
                    Globals::status->field_178 = 0;
                    return m;
                }
                break;
            case 0xa9:
                if (!atStation && extra > 2000) {
                    return m;
                }
                break;
            case 0xab:
                if (atStation && docked && station->getIndex() == m->getTargetStation()) {
                    return m;
                }
                break;
            case 0xac:
                if (atStation && docked && station->getIndex() == m->getTargetStation()) {
                    int index = m->getProductionGoodIndex();
                    int amount = m->getProductionGoodAmount();
                    if (Item::isInList(index, amount, ship->getCargo())) {
                        return m;
                    }
                }
                break;
        }
    }
    return 0;
}

void Status::loadAgents() {
    FileRead *fileRead = new FileRead();
    agents = fileRead->loadAgents();
    delete fileRead;
}

void Status::loadAgents(Array<int> *agents) {
    (void) agents;
}

static int g_levelXPTable[0x15] = {0, 7, 0x15, 0x2a, 0x46, 0x69, 0x93, 0xc4, 0xfc, 0x13b, 0x181, 0x1ce, 0x222, 0x27d, 0x2df, 0x348, 0x3b8, 0x42f, 0x4ad, 0x532, 0x672};

void Status::checkForLevelUp() {
    int sum = kills + field_d4 / 3 + field_a0 / 50 + field_a4 + missionCount * 2 +
              currentCampaignMission + stationsVisited;
    for (int i = 0; i != 0x15; i = i + 1) {
        if (sum >= g_levelXPTable[i]) {
            level = i;
        }
    }
}

void Status::moveWanted() {
    bool initialized = false;
    SystemPathFinder *pathFinder = 0;
    Array<SolarSystem *> *systems = 0;
    for (unsigned i = 0; i < this->wanted->size(); i++) {
        if (!(*this->wanted)[i]->isActive() || (*this->wanted)[i]->isTerminated() ||
            Globals::status->inAlienOrbit() ||
            (*this->wanted)[i]->getCurrentLocation() == Globals::status->station->getIndex()) {
            continue;
        }
        if (Level::programmedStation != 0 &&
            Level::programmedStation->getIndex() == (*this->wanted)[i]->getCurrentLocation()) {
            continue;
        }
        if (!initialized) {
            FileRead *fileRead = new FileRead();
            systems = fileRead->loadSystemsBinary();
            delete fileRead;
            pathFinder = new SystemPathFinder();
        }
        // Station lookups do not touch the reader instance, so no FileRead object is created.
        FileRead *reader = 0;
        Station *from = reader->loadStation((*this->wanted)[i]->getCurrentLocation());
        int fromSystem = from->getSystem();
        Station *to = reader->loadStation((*this->wanted)[i]->getTravelsTo());
        int toSystem = to->getSystem();
        Array<int> *path;
        if (from->getIndex() == to->getIndex()) {
            unsigned minJumps;
            unsigned maxJumps;
            if (i < 2) {
                maxJumps = 4;
                minJumps = 2;
            } else {
                int step = (int) (i - 1) % 6;
                minJumps = step / 3 + 2;
                maxJumps = 4 + step / 2;
            }
            (*this->wanted)[i]->setLastSeen((*this->wanted)[i]->getCurrentLocation());
            if (to != 0) {
                delete to;
            }
            to = Globals::globals->getRandomStation();
            toSystem = to->getSystem();
            while (true) {
                path = pathFinder->getSystemPath(systems, fromSystem, toSystem);
                uint32_t *routes = (*systems)[to->getSystem()]->getRoutes();
                if (path != 0 && routes != 0 && path->size() >= minJumps && path->size() <= maxJumps &&
                    (*Globals::status->systemVisibilities)[to->getSystem()] && to->getSystem() != 27 &&
                    to->getSystem() != 28 && to->getSystem() != 25 && to->getSystem() != 26 &&
                    to->getSystem() != 6 && to->getSystem() != from->getSystem()) {
                    (*this->wanted)[i]->setTravelsTo(to->getIndex());
                    break;
                }
                if (to != 0) {
                    delete to;
                }
                to = Globals::globals->getRandomStation();
                toSystem = to->getSystem();
            }
        } else if (fromSystem == toSystem) {
            (*this->wanted)[i]->setCurrentLocation((*this->wanted)[i]->getTravelsTo());
            path = 0;
        } else {
            path = pathFinder->getSystemPath(systems, fromSystem, toSystem);
            int gateIndex = (*systems)[(*path)[1]]->getWarpGateIndex();
            (*this->wanted)[i]->setCurrentLocation(gateIndex);
        }
        if (to != 0) {
            delete to;
        }
        if (from != 0) {
            delete from;
        }
        if (path != 0) {
            delete path;
        }
        initialized = true;
    }
    if (systems != 0) {
        ArrayReleaseClasses(*systems);
        delete systems;
    }
    if (pathFinder != 0) {
        delete pathFinder;
    }
}

void Status::setWingmen(Array<String *> *list) {
    if (wingmen != 0) {
        ArrayReleaseClasses(*wingmen);
    }
    if (list == 0) {
        wingmen = 0;
        reinterpret_cast<int32_t &>(this->_byte_0x28) = 0;
        this->fadeValue = 0;
    } else {
        wingmen = new Array<String *>();
        ArraySetLength(list->size(), *wingmen);
        for (unsigned i = 0; i < list->size(); i = i + 1) {
            String *name = new String(*(*list)[i], false);
            (*wingmen)[i] = name;
        }
    }
}
