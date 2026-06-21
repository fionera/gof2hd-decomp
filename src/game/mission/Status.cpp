#include "game/mission/Status.h"
Status* gStatus = nullptr;            // canonical Status singleton
#include "game/mission/PendingProduct.h"
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

// Engine/game globals and singletons reached through PC-relative slots in the
// shipped binary. They are owned elsewhere; Status only reads/writes through them.
extern int* g_campaignSentinel;
extern void* g_incKillsHook;
extern void* g_incPirateKillsHook;
extern void incKills_notify(void* arg);
extern void incPirateKills_notify(void* arg);

// FileRead — the binary save/data loader. It lives in its own translation unit;
// here we only need the few entry points used to materialise the wanted/agent/
// system/station tables. It is genuinely headerless from Status's point of view
// (its own header defines colliding layout-stub structs), so a minimal interface
// declaration is kept local.
struct FileRead {
    FileRead();
    ~FileRead();
    int loadStationsBinary();
    Array<SolarSystem*>* loadSystemsBinary();
    Array<Wanted*>* loadWanted();
    Array<Agent*>* loadAgents();
};

// Generator — builds station inventories (wares, ships, agents). Same headerless
// situation as FileRead.
struct Generator {
    Generator();
    Array<Agent*>* createAgents(Station* station);
    Array<Ship*>* getShipBuyList(Station* station);
    Array<Item*>* getItemBuyList(Station* station);
};

// SystemPathFinder — galaxy-graph route search. Its own header defines colliding
// layout-stub structs (Status/SolarSystem), so a minimal interface is kept local.
struct SystemPathFinder {
    SystemPathFinder();
    ~SystemPathFinder();
    Array<int>* getSystemPath(Array<SolarSystem*>* systems, int from, int to);
};

static SolarSystem* asSystem(int32_t v) { return (SolarSystem*)(intptr_t)v; }

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

// Returns 1 only when stranded in the supernova solar system before the late campaign.
int Status::inSupernovaSystem() {
    if (inAlienOrbit()) return 0;
    int idx = asSystem(system)->getIndex();
    int result = 0;
    if (idx == 0x1b && currentCampaignMission < 0x9e) result = 1;
    return result;
}

// Static HUD predicate: the game's only "challenge mode" is the supernova
// challenge, so this resolves the live status from the global singleton and
// reports whether the player is currently stranded in the supernova system.
int Status::isChallengeMode() {
    if (gStatus == 0) return 0;
    return gStatus->inSupernovaSystem();
}

void Status::visitStation() { stationsVisited = stationsVisited + 1; }

int Status::getMaxMissions() { return 2; }

void Status::setPirateKills(int v) { pirateKills = v; }

void Status::setFreelanceMission(Mission* m) {
    (*missions)[1] = m;
}

Array<Station*>* Status::getStationStack() {
    return stationStack;
}

Mission* Status::getMission() { return mission; }

void Status::setPassengers(int p) { passengers = p; }

int Status::getMissionCount() { return missionCount; }

void Status::setCredits(int v) { credits = v; }

bool Status::stringHasToken(String haystack, String needle) {
    return (int)haystack.IndexOf(needle) > -1;
}

int Status::getLastXP() { return lastXP; }

void Status::setLastXP(int v) { lastXP = v; }

void Status::setStationsVisited(int v) { stationsVisited = v; }

void Status::setStationStack(Array<Station*>* stack) { stationStack = stack; }

// The difficulty record lives in a game-settings singleton; difficulty 1.5 marks
// hard-core mode.
struct HardCoreHolder { char pad[0x2c]; float difficulty; };
extern HardCoreHolder* g_hardCoreHolder;
bool Status::hardCoreMode() {
    return g_hardCoreHolder->difficulty == 1.5f;
}

void Status::crateCaptured(int delta) { capturedCrates = delta + capturedCrates; }

int Status::getStanding() { return (int)(intptr_t)standing; }

int Status::getBoughtEquipment() { return boughtEquipment; }

int Status::getCredits() { return credits; }

void Status::setRating(int v) { rating = v; }

Array<Wanted*>* Status::getWanted() { return wanted; }

int Status::getCurrentCampaignMission() { return currentCampaignMission; }

void Status::setMissionCount(int v) { missionCount = v; }

void Status::setShip(Ship* s) {
    if (ship != 0) {
        delete ship;
        ship = 0;
    }
    ship = s;
}

Array<Mission*>* Status::getMissions() { return missions; }

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

int Status::getPendingProducts() { return (int)(intptr_t)pendingProducts; }

Array<bool>* Status::getSystemVisibilities() { return systemVisibilities; }

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

Station* Status::getStation() { return station; }

void Status::setCampaignMission(Mission* m) {
    m->setCampaignMission(true);
    Mission** slot = missions->data();
    Mission* cur = slot[0];
    if (cur != 0 && (int*)cur != g_campaignSentinel) {
        delete cur;
        slot[0] = 0;
    }
    slot[0] = m;
}

// Returns the active, non-terminated wanted target located in the current orbit
// with the lowest required-bounty count.
Wanted* Status::getWantedInCurrentOrbit() {
    Wanted* best = 0;
    if (wanted != 0) {
        for (unsigned i = 0; i < wanted->size(); i = i + 1) {
            Wanted* w = (*wanted)[i];
            if (w->isActive() != 0 && w->isTerminated() == 0) {
                if (w->getCurrentLocation() == station->getIndex()) {
                    if (best == 0 || best->getRequiredBounties() < w->getRequiredBounties()) {
                        best = w;
                    }
                }
            }
        }
    }
    return best;
}

int Status::getLevel() { return level; }

// Releases the ship, campaign mission, agent/station-stack/wanted arrays.
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
        for (auto* e : *agents) delete e;
        delete agents;
    }
    agents = 0;
    if (stationStack != 0) {
        for (auto* e : *stationStack) delete e;
        delete stationStack;
    }
    stationStack = 0;
    if (wanted != 0) {
        for (auto* e : *wanted) delete e;
        delete wanted;
    }
    wanted = 0;
}

bool Status::inStormOrbit() {
    if (inAlienOrbit()) return false;
    if (currentCampaignMission < 0x5a) return false;
    if (inSupernovaSystem() != 0) return true;
    if (asSystem(system)->getTextureIndex() == 0x10) return true;
    return asSystem(system)->getTextureIndex() == 0x12;
}

int Status::getCapturedCrates() { return capturedCrates; }

void Status::incKills() {
    kills = kills + 1;
    return incKills_notify(g_incKillsHook);
}

int Status::getStationsVisited() { return stationsVisited; }

int Status::getCampaignMission() {
    return (int)(intptr_t)(*missions)[0];
}

// Mission*-typed view of the same slot getCampaignMission() returns: the first
// entry of the missions array is the active campaign mission.
Mission* Status::getCampaignMissionPtr() {
    return (*missions)[0];
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

// Counts the non-empty slots in the mission array.
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

// True if any wanted entry can post on the current system's bounty board right now.
int Status::wantedBoardAccessible() {
    Status* st = gStatus;
    Array<Wanted*>* w = st->wanted;
    for (unsigned i = 0; i < w->size(); i = i + 1) {
        int race = asSystem(st->system)->getRace();
        int board = (*w)[i]->getBoard();
        if (race == board) {
            if (st->currentCampaignMission >= (*w)[i]->getRequiredMission() &&
                st->inAlienOrbit() == 0 &&
                st->station->getIndex() != 0x6c) {
                return 1;
            }
        }
    }
    return 0;
}

void Status::incPirateKills() {
    pirateKills = pirateKills + 1;
    return incPirateKills_notify(g_incPirateKillsHook);
}

void Status::setGoodsProduced(int v) { goodsProduced = v; }

void Status::setMission(Mission* m) { mission = m; }

int Status::getCollectedBounties(int index) {
    if (index < 4) return collectedBounties[index];
    return 0;
}

int Status::getRating() { return rating; }

Mission* Status::getFreelanceMission() {
    return (*missions)[1];
}

void Status::jumpgateUsed() { jumpgatesUsed = jumpgatesUsed + 1; }

Ship* Status::getShip() { return ship; }

int Status::getPlanetTextures() { return planetTextures; }

int Status::getPassengers() { return passengers; }

bool Status::isStorylineWanted(int index) { return (unsigned)index < 2; }

// Returns the first failed mission, or a type-0xd mission flagged failable.
int Status::missionFailed(bool docked, int64_t time) {
    for (unsigned i = 0; i < missions->size(); i = i + 1) {
        Mission* cur = (*missions)[i];
        if (cur->hasFailed() != 0) {
            return 0;
        }
        if (cur != 0 && cur->getType() == 0xd && docked && this->field_0x111 != 0) {
            return (int)(intptr_t)cur;
        }
    }
    return 0;
}

int Status::getBluePrints() { return (int)(intptr_t)bluePrints; }

int Status::getAgents() { return (int)(intptr_t)agents; }

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

int Status::getWingmen() { return wingmen; }

int Status::getGoodsProduced() { return goodsProduced; }

int Status::getKills() { return kills; }

// Credit deltas whose magnitude exceeds 1e9 are ignored; the balance is clamped at >= 0.
void Status::changeCredits(int delta) {
    int magnitude = delta < 0 ? -delta : delta;
    if (magnitude <= 1000000000) {
        int credited = delta + credits;
        credits = credited & ~(credited >> 0x1f);
    }
}

// True when `station` hosts a freighter mission (two bit-masks plus the special id 0xf).
// The masks pick out the freighter stations relative to the 0x1e / 0x46 bases
// (the set bits match getFreighterMissionStationBit's station list).
static const int kFreighterMaskA = 0x40008401;   // stations 0x1e,0x28,0x2d,0x3c
static const int kFreighterMaskB = 0x02008401;   // stations 0x46,0x50,0x55,0x5f

int Status::isFreighterMissionStation(int station) {
    if ((unsigned)(station - 0x1e) < 0x1f && ((1 << ((station - 0x1e) & 0xff)) & kFreighterMaskA) != 0)
        return 1;
    if ((unsigned)(station - 0x46) < 0x1a && ((1 << ((station - 0x46) & 0xff)) & kFreighterMaskB) != 0)
        return 1;
    if (station == 0xf) return 1;
    return 0;
}

// Returns whether the blueprint with item index `index` has been unlocked.
int Status::isBlueprintUnlocked(int index) {
    unsigned i = 0;
    while (true) {
        if (bluePrints->size() <= i) {
            return 0;
        }
        if ((*bluePrints)[i]->getIndex() == index) {
            break;
        }
        i = i + 1;
    }
    return (*bluePrints)[i]->isUnlocked();
}

// Allocates the mission/station/visibility containers, sizes them, and
// zero-initializes the persistent player state to its new-game defaults.
Status::Status() {
    missions = new Array<Mission*>();
    stationStack = new Array<Station*>();
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
    missions->resize(2);
    stationStack->resize(3);
    systemVisibilities->resize(0x22);
    field_94->resize(0xb);
    field_98->resize(0xb);
    field_ac->resize(0x16);
    field_b4->resize(0x22);
    field_4c->resize(4);
    field_50->resize(0xf);
    field_54->resize(0xe9);
    field_58->resize(5);
    stationsVisited = 0;
    currentCampaignMission = 0;
    passengers = 0;
    field_10c = 0;
    field_110 = 0;
    kills = 0;
    missionCount = 0;
    level = 1;
    credits = 0;
    rating = 0;
    playingTime = 0;
    playerStation = new Station();
    field_8c = 0;
    ship = 0;
    mission = 0;
    station = 0;
    system = 0;
    planetNames = 0;
    planetTextures = 0;
    voidStation = nullptr;
    wanted = 0;
    wingmen = 0;
    this->field_0x28 = 0;
    standing = 0;
    bluePrints = 0;
    pendingProducts = 0;
    agents = 0;
    this->field_0x3c = 0;
    this->field_0x40 = 0;
    this->field_0x44 = 0;
    this->field_0x48 = 0;
    collectedBounties[0] = 0;
    collectedBounties[1] = 0;
    collectedBounties[2] = 0;
    collectedBounties[3] = 0;
    this->field_0x17c = 0;
    field_178 = 0;
}

// Maps a station id to its bit position in the freighter-mission visited mask (0 if none).
int Status::getFreighterMissionStationBit(int station) {
    if (station == 0xf) return 4;
    if (station == 0x5f) return 6;
    if (station == 0x28) return 3;
    if (station == 0x2d) return 9;
    if (station == 0x3c) return 5;
    if (station == 0x46) return 7;
    if (station == 0x50) return 8;
    if (station == 0x55) return 1;
    if (station == 0x1e) return 2;
    return 0;
}

extern int** g_ccpPriceMod;

// Status::calcCargoPrices()
//   Reprices every item in the player's cargo, equipment, and current station
//   inventory based on the galactic distance between the item's cheapest/most-
//   expensive systems and the current system, clamped to the item's price band
//   with a small per-system random jitter.
void Status::calcCargoPrices() {
    Galaxy* gal = gGalaxy;
    Array<SolarSystem*>* systems = gal->getSystems();
    AbyssEngine::AERandom* rng = gRandom;
    const float kPriceScale = 100.0f;  // pool @0x000bccac = 0x42c80000
    const float kJitter = 0.02f;        // pool @0x000bccb0 = 0x3ca3d70a

    for (int src = 0; src != 3; src = src + 1) {
        Array<Item*>* list;
        if (src == 1)
            list = ship->getEquipment();
        else if (src == 0)
            list = ship->getCargo();
        else
            list = station->getItems();
        if (list == 0)
            continue;

        // seed by the station index so prices are deterministic per visit.
        rng->setSeed((long long)station->getIndex());
        bool ringWorld = false;
        if (!inAlienOrbit())
            ringWorld = (asSystem(system)->getIndex() == 0x19);

        for (unsigned i = 0; i < list->size(); i = i + 1) {
            Item* item = (*list)[i];
            if (item == 0)
                continue;

            SolarSystem* itemSys = (*systems)[item->getIndex()];
            SolarSystem* maxSys = (*systems)[item->getMaxPriceSystem()];
            int dItem = gal->distancePercent(itemSys->getX(), itemSys->getY(),
                                             maxSys->getX(), maxSys->getY());

            SolarSystem* curSys = (*systems)[station->getIndex()];
            int dHere = gal->distancePercent(itemSys->getX(), itemSys->getY(),
                                             curSys->getX(), curSys->getY());

            float t = ((kPriceScale / (float)dItem) * (float)dHere) / kPriceScale;
            int minPrice = item->getMinPrice();
            int band = item->getMaxPrice() - item->getMinPrice();
            float clamp = t < 1.0f ? t : 1.0f;

            if (item->getSinglePrice() > 0) {
                int price;
                if (ringWorld) {
                    price = item->getMaxPrice();
                } else {
                    int base = minPrice + (int)(clamp * (float)band);
                    int jitterMax = (int)((float)base * kJitter);
                    if (jitterMax < 2)
                        jitterMax = 1;
                    price = (base - jitterMax) + rng->nextInt((jitterMax << 1) | 1);
                }
                if (*g_ccpPriceMod != 0 && **g_ccpPriceMod != 0) {
                    // apply a global price modifier when present.
                    // price + (price * priceMod) * 0.01f   (pool @0x000bccb4 = 0x3c23d70a)
                    price = (int)((float)price + (float)price * (float)(**g_ccpPriceMod) * 0.01f);
                }
                item->setPrice(price);
            }
        }
        rng->reset();
    }
}

// Returns the matching stack slot's station (truthy) if `s` is already on the
// 3-deep station stack, else 0.
bool Status::isOnStack(Station* s) {
    unsigned i = 0;
    while (true) {
        if (2 < i) {
            return 0;
        }
        Station* cur = (*stationStack)[i];
        if (cur != 0 && cur->equals(s) != 0) {
            break;
        }
        i = i + 1;
    }
    return (*stationStack)[i] != 0;
}

// True iff the orbited station differs from the player's home station.
bool Status::inAlienOrbit_impl(Station* station, Station* playerStation) {
    return station != playerStation;
}

bool Status::inAlienOrbit() {
    return Status::inAlienOrbit_impl(station, playerStation);
}

// Global slots referenced by resetGame.
extern int* g_rg_settings;                 // -> settings record
extern Array<Item*>** g_rg_itemTable;
extern int** g_rg_statusSlotC;
extern int** g_rg_zeroSlotA;
extern char** g_rg_zeroSlotB;
extern int** g_rg_zeroSlotC;
extern int (*g_rg_makeItemB)(int);
extern void (*g_rg_addCargo)(int, int, int);

#include "game/core/Globals.h"

// Status::resetGame()
//   Wipes the persistent player state back to a fresh new-game baseline: zeroes
//   counters, recreates the bonus/visibility arrays, blueprint list, agents/
//   wanted, standing, the starting mission, ship and station.
void Status::resetGame() {
    int* settings = g_rg_settings;
    char* srec = (char*)*settings;

    this->field_8c = 0;
    this->boughtEquipment = 0;
    this->field_0x100 = 0;
    this->field_0x104 = 0;
    this->credits = 0;
    this->rating = 0;
    this->playingTime = 0;
    this->kills = 0;
    this->missionCount = 0;
    this->level = 1;
    this->lastXP = 0;
    this->stationsVisited = 0;
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
    this->field_13c = 0;
    this->field_134 = 0;
    this->field_144 = 0;
    this->field_120 = 0;
    this->field_128 = 0;
    this->field_130 = 0;
    this->field_138 = 0;
    this->field_140 = 0;
    this->field_148 = 0;

    char hardcore = *(char*)(srec + 0x36);
    this->field_0x111 = 0;
    this->field_114 = hardcore != 0 ? 3 : 0;

    // recreate the per-game scratch "void" station.
    if (this->voidStation != nullptr) {
        delete this->voidStation;
        this->voidStation = nullptr;
    }
    this->voidStation = new Station();

    if (this->field_0x28 != 0) {
        delete[] (char*)(intptr_t)this->field_0x28;
        this->field_0x28 = 0;
    }
    if (this->wingmen != 0) {
        Array<String*>* wm = (Array<String*>*)(intptr_t)this->wingmen;
        for (auto* e : *wm) delete e;
        delete wm;
    }

    this->field_0x30 = 0;
    this->wingmen = 0;
    for (unsigned j = 0; j < this->field_94->size(); j = j + 1)
        (*this->field_94)[j] = 0;

    for (unsigned j = 0; j < this->field_98->size(); j = j + 1)
        (*this->field_98)[j] = 0;

    this->field_9c = 0;
    this->field_a0 = 0;
    this->field_a4 = 0;
    this->field_a8 = 0;
    for (unsigned j = 0; j < this->field_ac->size(); j = j + 1)
        (*this->field_ac)[j] = 0;

    this->field_b0 = 0;
    for (unsigned j = 0; j < this->field_b4->size(); j = j + 1)
        (*this->field_b4)[j] = 0;

    // free any stations on the stack.
    if (this->stationStack != 0) {
        for (unsigned k = 0; k < this->stationStack->size(); k = k + 1) {
            Station* st = (*this->stationStack)[k];
            if (st != 0) {
                delete st;
                (*this->stationStack)[k] = 0;
            }
        }
    }

    gGlobals->resetHints();
    Galaxy* gal = gGalaxy;
    gal->reset();

    for (unsigned j = 0; j < this->field_50->size(); j = j + 1)
        (*this->field_50)[j] = 0;

    // array at 0x54: first 0xb0 entries = 1, rest = 0.
    Array<bool>& arr54 = *this->field_54;
    for (int j = 0; j != 0xb0; j = j + 1)
        arr54[j] = 1;
    for (unsigned k = 0xb0; k < arr54.size(); k = k + 1)
        arr54[k] = 0;

    arr54[0x3e] = 0;
    arr54[0x3c] = 0;
    arr54[0x3d] = 0;
    this->passengers = 0;
    this->field_b8 = 0;
    // zero the two ship-equipment-slot vectors.
    this->field_e0 = 0; this->field_e4 = 0; this->field_e8 = 0; this->field_ec = 0;
    this->field_c0 = 0; this->field_c4 = 0; this->field_c8 = 0; this->field_cc = 0;
    this->field_d0 = 0; this->field_d4 = 0; this->field_d8 = 0; this->field_dc = 0;

    // a few scalar globals get zeroed.
    **g_rg_zeroSlotA = 0;
    **g_rg_zeroSlotB = 0;
    **g_rg_zeroSlotC = 0;

    this->field_160 = 0;
    this->field_164 = 0;
    this->field_0x108 = 0;
    this->field_174 = 0;
    this->field_0xf0 = 0;
    this->field_f4 = -1;
    this->field_f8 = 1;

    gAchievements->init();

    // recreate the four int arrays at 0x40/0x3c/0x48/0x44 (length 0xe9).
    Array<int>** off4[4] = {&this->field_0x40, &this->field_0x3c,
                            &this->field_0x48, &this->field_0x44};
    for (int q = 0; q < 4; q = q + 1) {
        delete *off4[q];
        *off4[q] = 0;
    }
    for (int q = 0; q < 4; q = q + 1) {
        *off4[q] = new Array<int>();
        (*off4[q])->resize(0xe9);
    }
    {
        Array<int>& d40 = *this->field_0x40;
        Array<int>& d3c = *this->field_0x3c;
        Array<int>& d48 = *this->field_0x48;
        Array<int>& d44 = *this->field_0x44;
        for (int j = 0; j != 0xe9; j = j + 1) {
            d40[j] = 0; d3c[j] = 0; d48[j] = 0; d44[j] = 0;
        }
    }

    // bool array at 0x4c (length 4).
    delete this->field_4c;
    this->field_4c = new Array<bool>();
    this->field_4c->resize(4);
    for (int j = 0; j != 4; j = j + 1) (*this->field_4c)[j] = 0;

    // bool array at 0x58 (length 5).
    delete this->field_58;
    this->field_58 = new Array<bool>();
    this->field_58->resize(5);
    for (int j = 0; j != 5; j = j + 1) (*this->field_58)[j] = 0;

    // system visibilities from the galaxy systems.
    Array<SolarSystem*>* systems = gal->getSystems();
    Array<bool>& vis = *this->systemVisibilities;
    for (unsigned k = 0; k < systems->size(); k = k + 1) {
        vis[k] = (*systems)[k]->isVisible() != 0;
    }

    // count craftable blueprints.
    unsigned bpCount = 0;
    Array<Item*>* items = *g_rg_itemTable;
    for (unsigned k = 0; k < items->size(); k = k + 1) {
        if ((*items)[k]->getIngredients() != 0)
            bpCount = bpCount + 1;
    }

    if (this->bluePrints != 0) {
        for (auto* e : *this->bluePrints) delete e;
        delete this->bluePrints;
    }
    this->bluePrints = 0;
    if (bpCount != 0) {
        this->bluePrints = new Array<BluePrint*>();
        this->bluePrints->resize(bpCount);
        int idx = 0;
        for (unsigned k = 0; k < items->size(); k = k + 1) {
            if ((*items)[k]->getIngredients() != 0) {
                (*this->bluePrints)[idx] = new BluePrint(k);
                idx = idx + 1;
            }
        }
    }

    if (this->pendingProducts != 0) {
        for (auto* e : *this->pendingProducts) delete e;
        delete this->pendingProducts;
    }
    this->pendingProducts = 0;

    loadAgents();
    loadWanted();

    // recreate Standing.
    if (this->standing != 0) {
        delete this->standing;
        this->standing = 0;
    }
    this->standing = new Standing();
    this->field_7c = -1;
    this->field_80 = -1;
    this->field_84 = 0;
    // missions[0] gets a base value from a status slot.
    (*this->missions)[0]->setType(**g_rg_statusSlotC);
    this->currentCampaignMission = 0;

    setCampaignMission(new Mission(4, 0, 0x4e));

    this->mission = (*this->missions)[0];
    setShip(gStatus->ship->makeShip(-1));
    this->ship->priceDecline();
    setStation((Station*)(intptr_t)gal->getStation(0));  // new-game home station (index 0)
    this->ship->setCargo(0);

    int (*makeItemB)(int) = g_rg_makeItemB;
    void (*addCargo)(int, int, int) = g_rg_addCargo;
    int shipObj = (int)(intptr_t)this->ship;
    int* srcShip = (int*)(*items->data());

    addCargo(shipObj, makeItemB(srcShip[2]), 0);
    addCargo(shipObj, makeItemB(srcShip[2]), 1);
    addCargo(shipObj, makeItemB(srcShip[0x36]), 0);
    addCargo(shipObj, makeItemB(srcShip[0x3b]), 1);
    addCargo(shipObj, makeItemB(srcShip[0x52]), 2);
    addCargo(shipObj, makeItemB(srcShip[0x49]), 3);
    addCargo((int)(intptr_t)gStatus->ship,
             (int)(intptr_t)((Item*)(intptr_t)srcShip[0x24])->makeItem(), 0);

    if (*(char*)(srec + 0x35) != 0)
        (*this->systemVisibilities)[0x19] = 1;

    this->field_64 = this->ship->getMaxHP();
    this->field_5c = this->ship->getMaxShieldHP();
    this->field_60 = this->ship->getMaxArmorHP();
    this->field_68 = 100;
    this->field_150 = -1;
    this->field_154 = -1;
    this->field_158 = -1;
}

extern int g_anwSysMask;

// Walks the wanted roster and activates any newly-eligible bounty, routing it
// onto a random reachable station pair. Returns the number of bounties activated.
int Status::activateNewWanted() {
    Status* slot = gStatus;
    Array<Wanted*>* w = slot->wanted;
    if (w == 0) {
        return 0;
    }
    Array<SolarSystem*>* systems = 0;
    int activated = 0;
    SystemPathFinder* pf = 0;
    for (unsigned i = 0; i < w->size(); i = i + 1) {
        Wanted* cur = (*w)[i];
        if (cur == 0) {
            continue;
        }
        unsigned lo, hi;
        if (i < 2) {
            hi = 4;
            lo = 2;
        } else {
            int rem = (i - 1) % 6;
            lo = rem / 3 + 2;
            hi = rem / 2 + 4;
        }
        if (asSystem(slot->system)->getRace() != cur->getBoard()) {
            continue;
        }
        if (slot->currentCampaignMission < cur->getRequiredMission()) {
            continue;
        }
        if (i < 2 && slot->currentCampaignMission > cur->getRequiredMission()) {
            continue;
        }
        if (slot->inAlienOrbit() != 0) {
            continue;
        }
        if (slot->station->getIndex() == 0x6c) {
            continue;
        }
        if (cur->isActive() != 0) {
            continue;
        }
        if (cur->isTerminated() != 0) {
            continue;
        }
        int board = cur->getBoard();
        int avail = (board < 4) ? slot->collectedBounties[board] : 0;
        if (avail < cur->getRequiredBounties()) {
            continue;
        }
        cur->setActive(true);
        if (pf == 0) {
            pf = new SystemPathFinder();
            FileRead fr;
            systems = fr.loadSystemsBinary();
        }
        activated = activated + 1;
        Array<int>* path;
        int fromSys, toSys;
        do {
            Station* a = gGlobals->getRandomStation();
            unsigned asys = a->getSystem();
            Array<bool>* vis = slot->systemVisibilities;
            while (true) {
                bool ok = false;
                if (vis != 0 && asys < vis->size()) {
                    ok = (*vis)[asys] != 0;
                }
                if (asSystem((int)asys)->getRoutes() != 0 && ok &&
                    !(asys < 0x1d && (1 << (asys & 0xff) & g_anwSysMask) != 0)) {
                    break;
                }
                if (a != 0) {
                    delete a;
                }
                a = gGlobals->getRandomStation();
                asys = a->getSystem();
            }
            Station* b = gGlobals->getRandomStation();
            unsigned bsys = b->getSystem();
            while (true) {
                bool ok2 = false;
                if (vis != 0 && bsys < vis->size()) {
                    ok2 = (*vis)[bsys] != 0;
                }
                if (asSystem((int)bsys)->getRoutes() != 0 && ok2 &&
                    !(bsys < 0x1d && (1 << (bsys & 0xff) & g_anwSysMask) != 0) && asys != bsys) {
                    break;
                }
                if (b != 0) {
                    delete b;
                }
                b = gGlobals->getRandomStation();
                bsys = b->getSystem();
            }
            cur->setLastSeen(a->getIndex());
            cur->setTravelsTo(b->getIndex());
            fromSys = a->getSystem();
            toSys = b->getSystem();
            if (a != 0) {
                delete a;
            }
            if (b != 0) {
                delete b;
            }
            path = pf->getSystemPath(systems, fromSys, toSys);
        } while (path == 0 || (unsigned)(*path)[0] < lo || hi < (unsigned)(*path)[0]);
        AbyssEngine::AERandom* rnd = gRandom;
        int pick = (*path)[rnd->nextInt(path->size())];
        SolarSystem* dst = (*systems)[pick];
        Array<int>* dstStations = (Array<int>*)dst->getStations();
        if (dstStations != 0) {
            int idx = rnd->nextInt(dstStations->size());
            int st = (*dstStations)[idx];
            cur->setCurrentLocation(st);
        }
        delete path;
    }
    if (pf != 0) {
        delete pf;
    }
    if (systems != 0) {
        for (auto* e : *systems) delete e;
        delete systems;
    }
    return activated;
}

// Merges `bp` into the pending-products list, bumping an existing matching entry's
// quantity or appending a freshly constructed PendingProduct.
void Status::addPendingProduct(BluePrint* bp) {
    if (pendingProducts == 0) {
        pendingProducts = new Array<PendingProduct*>();
    } else {
        for (unsigned i = 0; i < pendingProducts->size(); i = i + 1) {
            PendingProduct* e = (*pendingProducts)[i];
            if (e != 0 && e->blueprintIndex == bp->getIndex() &&
                e->stationIndex == bp->getStationIndex()) {
                e->quantity = bp->getQuantity() + e->quantity;
                return;
            }
        }
    }
    PendingProduct* pp = new PendingProduct(bp);
    pendingProducts->push_back(pp);
}

extern int g_emptyOrbitMask;

// True when the current orbit has no station/special content to interact with.
bool Status::inEmptyOrbit() {
    int idx = station->getIndex();
    if (idx == 0x4e && currentCampaignMission < 2) {
        return true;
    }
    if (inAlienOrbit() != 0 && (unsigned)(currentCampaignMission - 0x2b) < 0x29) {
        return true;
    }
    if (inAlienOrbit() != 0 && currentCampaignMission > 0x99) {
        return true;
    }
    unsigned d = idx - 0x66;
    if (d < 0x20) {
        if ((1 << (d & 0xff) & g_emptyOrbitMask) != 0) {
            return true;
        }
        if (d == 9) {
            if (currentCampaignMission > 0x5d) {
                return true;
            }
            return idx == 0x86;
        }
    }
    if (idx != 0x65 || currentCampaignMission < 0x54) {
        return idx == 0x86;
    }
    return true;
}

// Reads the wanted/bounty list from disk via a transient FileRead helper.
void Status::loadWanted() {
    FileRead fr;
    wanted = fr.loadWanted();
}

// Unlocks every blueprint whose item index matches `index`.
void Status::unlockBluePrint(int index) {
    for (unsigned i = 0; i < bluePrints->size(); i = i + 1) {
        BluePrint* bp = (*bluePrints)[i];
        if (bp->getIndex() == index) {
            bp->unlock();
        }
    }
}

// Per-second gamma-ray damage near supernova/storm orbits, keyed by station/system
// ids. Returns the float result as raw bits (the engine treats it as float at the
// call site).
extern const float g_gammaTableA[5];
extern const float g_gammaTableB[5];

static inline int as_int(float f) { union { float f; int i; } u; u.f = f; return u.i; }
static inline float as_float(unsigned u) { union { unsigned u; float f; } x; x.u = u; return x.f; }

int Status::getGammaRayDamagePerSecond(int station, int system) {
    unsigned k = station - 0x6d;
    float result = as_float(0x00000000u);
    if (k < 5) {
        if (system < 0x6a) {
            if (k < 5) return as_int(g_gammaTableA[k]);
        } else if (currentCampaignMission < 0x9e) {
            if (k < 5) return as_int(g_gammaTableB[k]);
        } else if (station == 0x6d) {
            result = as_float(0x3f800000u);
        }
    }
    return as_int(result);
}

// Pushes `s` onto the 3-deep station stack (shifting older entries down), unless
// it is already present. Returns 1 if the stack was modified, 0 otherwise.
int Status::addStationToStack(Station* s) {
    if (isOnStack(s)) {
        setStationTail(s);
        return 0;
    }
    Station** base = stationStack->data();
    if (base[0] == 0) {
        int j;
        int i = 0;
        do {
            j = i;
            if (j + 2 < 0) {
                return 0;
            }
            i = j - 1;
        } while (base[j + 2] != 0);
        base[j + 2] = s;
    } else {
        if (base[2] != 0) {
            delete base[2];
        }
        base[2] = 0;
        for (int i = 0; i != -2; i = i - 1) {
            base[i + 2] = base[i + 1];
        }
        base[0] = s;
    }
    setStationTail(s);
    return 1;
}

extern int* g_missionSentinel;

// Removes `mission` from the active list: clears the campaign slot, detaches its
// agent, and resets the slot to the sentinel value.
void Status::removeMission(Mission* mission) {
    int* sentinel = g_missionSentinel;
    Mission** slots = missions->data();
    for (unsigned i = 0; i < missions->size(); i = i + 1) {
        if (slots[i] == mission) {
            Mission* target = mission;
            if (this->mission == mission) {
                mission = 0;
                this->mission = 0;
                target = slots[i];
            }
            if (target->getAgent() != 0) {
                slots[i]->getAgent()->setMission(0);
            }
            if (slots[i] != 0) {
                delete slots[i];
            }
            slots[i] = (Mission*)(intptr_t)*sentinel;
        }
    }
}

extern int g_ncmAchTable[3];   // 3 mission ids

extern void Status_addMissionTail(Status* self, Mission* m);

// Per-campaign-step mission descriptor (the bulk of the switch is "build
// Mission(type,param,station) and register it" -- either as the campaign mission
// or via the add-mission tail).
struct Step { short type; int param; short station; unsigned char campaign; };

// Table indexed by campaign step (recovered from the engine's switch). Steps with
// extra side effects are handled specially below; the rest follow this descriptor.
static const Step kSteps[] = {
    /*0x00*/ {0, 0, 0, 0},
    /*0x01*/ {0x9a, 0, 0x4e, 1}, /*0x02*/ {0xb, 0, 0x4e, 0}, /*0x03*/ {0x9a, 0, 0x4e, 1},
    /*0x04*/ {0xb, 0, 0x4e, 0},  /*0x05*/ {0x9e, 0, 0x4e, 0}, /*0x06*/ {0x4, 0, 0x4e, 0},
    /*0x07*/ {0xb, 0, 0x4e, 0},  /*0x08*/ {0xb, 0, 0x4e, 0},  /*0x09*/ {0xb, 0, 0x4f, 0},
    /*0x0a*/ {0xb, 0, 0x4c, 0},  /*0x0b*/ {0xb, 0, 0x4f, 0},  /*0x0c*/ {0x96, 0, 0, 1},
    /*0x0d*/ {0x4, 0, 0x4f, 0},  /*0x0e*/ {0xb, 0, 0x62, 0},  /*0x0f*/ {0x4, 0, 0x62, 0},
    /*0x10*/ {0xb, 0, 0x62, 0},  /*0x11*/ {0x9c, 0, 0x38, 0}, /*0x12*/ {0x9c, 0, 0x37, 0},
    /*0x13*/ {0xbd, 0, 0x37, 1}, /*0x14*/ {0x4, 0, 0x37, 0},  /*0x15*/ {0xb, 0, 0x37, 0},
    /*0x16*/ {0xb, 20000, 10, 0},/*0x17*/ {0x4, 0, 0x30, 0},  /*0x18*/ {0x4, 0, 0x30, 0},
    /*0x19*/ {0x4, 0, 0x30, 0},  /*0x1a*/ {0xb, 0, 10, 0},    /*0x1b*/ {0x4, 0, 0x5b, 0},
    /*0x1c*/ {0x4, 0, -1, 0},    /*0x1d*/ {0x9c, 0, 0x5b, 0}, /*0x1e*/ {0xb, 30000, 0x62, 0},
    /*0x1f*/ {0xb, 0, 10, 0},    /*0x20*/ {0x8, 0, 10, 1},    /*0x21*/ {0xb, 0, 0x1d, 0},
    /*0x22*/ {0xb, 0, 0x1d, 0},  /*0x23*/ {0xc, 0, 0x1b, 0},  /*0x24*/ {0xa0, 0, 0x1b, 0},
    /*0x25*/ {0x4, 0, 0x16, 0},  /*0x26*/ {0xb, 0, 0x1e, 0},  /*0x27*/ {0xa1, 0, -1, 0},
    /*0x28*/ {0x4, 0, -1, 0},    /*0x29*/ {0xa0, 0, -1, 0},   /*0x2a*/ {0xb, 0, 10, 0},
    /*0x2b*/ {0xb, 0, 10, 0},    /*0x2c*/ {0, 0, 0, 1},       /*0x2d*/ {0xb, 0, 0x4a, 0},
    /*0x2e*/ {0xb, 0, 0x3a, 0},  /*0x2f*/ {0xb, 0, 0x3a, 0},  /*0x30*/ {0x9c, 0, 0x3a, 0},
    /*0x31*/ {0x9c, 0, 0x3e, 0}, /*0x32*/ {0x9c, 0, 0x19, 0}, /*0x33*/ {0xa0, 0, 0x19, 0},
    /*0x34*/ {0, 0, 0, 0},       /*0x35*/ {0xb, 0, 0x4a, 0},  /*0x36*/ {0xb, 0, 0x65, 1},
    /*0x37*/ {0x4, 0, 0x66, 0},  /*0x38*/ {0xb, 0, 0x65, 0},  /*0x39*/ {0xa6, 0, 0x65, 1},
    /*0x3a*/ {0xa3, 0, 0x65, 1}, /*0x3b*/ {0xb, 50000, 0x65, 1}, /*0x3c*/ {0xa4, 0, 0x65, 0},
    /*0x3d*/ {0xb, 0, 100, 0},   /*0x3e*/ {0x4, 0, 0x67, 0},  /*0x3f*/ {0x4, 0, 0x68, 0},
    /*0x40*/ {0xb, 0, 100, 0},   /*0x41*/ {0xb, 0, 0x65, 0},  /*0x42*/ {0x4, 0, 0x68, 1},
    /*0x43*/ {0x8, 0, 0x42, 1},  /*0x44*/ {0x6, 0, 0x42, 0},  /*0x45*/ {0x6, 0, 0x41, 0},
    /*0x46*/ {0xb, 0, 0x42, 0},  /*0x47*/ {0xa4, 0, 0x65, 1}, /*0x48*/ {0xa, 0, 0x51, 0},
    /*0x49*/ {0xb, 0, 100, 0},   /*0x4a*/ {0xb, 0, 100, 0},   /*0x4b*/ {0xb, 0, 100, 0},
    /*0x4c*/ {0xb, 0, 0x65, 0},  /*0x4d*/ {0x4, 0, 0x65, 0},  /*0x4e*/ {0xa5, 0, -1, 0},
    /*0x4f*/ {0x1, 0, 100, 0},   /*0x50*/ {0x4, 0, -1, 0},    /*0x51*/ {0xb, 0, 100, 0},
    /*0x52*/ {0xb, 0, 100, 0},   /*0x53*/ {0, 0, 0, 0},       /*0x54*/ {0xa4, 0, 0, 0},
    /*0x55*/ {0xb, 0, 100, 0},   /*0x56*/ {0x4, 0, 10, 0},    /*0x57*/ {0xb, 0, 10, 0},
    /*0x58*/ {0x4, 0, 0x6d, 0},
};

// Status::nextCampaignMission()
//   Advances the campaign by one step: bumps the campaign-mission counter,
//   snapshots the playing-time, runs the achievement check, then spawns and
//   registers the next scripted Mission for that step (with a few steps
//   performing extra cargo/equipment bookkeeping).
void Status::nextCampaignMission() {
    int prevTimeLo = (int)this->playingTime;
    int prevTimeHi = (int)(this->playingTime >> 32);

    int step = this->currentCampaignMission;
    int next = step + 1;

    // achievement check: flag if the new step matches any tracked mission id.
    for (int k = 0; k < 3; k = k + 1) {
        if (next == g_ncmAchTable[k])
            this->field_0x17c = 1;
    }

    this->currentCampaignMission = next;
    this->field_0x100 = prevTimeLo;
    this->field_0x104 = prevTimeHi;

    if (step == 0)
        return;   // step 0 has no mission.

    // --- steps with extra side effects ------------------------------------
    if (step == 3) {
        this->ship->setCargo(0);
    } else if (step == 5) {
        this->ship->removeAllCargo();
    } else if (step == 7) {
        Array<Item*>* eq = this->ship->getEquipment();
        if (eq != 0)
            for (unsigned i = 0; i < eq->size(); i = i + 1) {
                Item* it = (*eq)[i];
                if (it != 0) { it->setUnsaleable(false); it->setPrice(it->getSinglePrice()); }
            }
        Array<Item*>* cg = this->ship->getCargo();
        if (cg != 0)
            for (unsigned i = 0; i < cg->size(); i = i + 1) {
                Item* it = (*cg)[i];
                if (it != 0) { it->setUnsaleable(false); it->setPrice(it->getSinglePrice()); }
            }
    } else if (step == 9) {
        Item* old = this->ship->getFirstEquipmentOfSort(0);
        this->ship->removeEquipment(old);
        this->ship->addEquipment(((Item*)0)->makeItem());  // item id 0x158
    }

    // --- spawn + register the step's mission -------------------------------
    if ((unsigned)step < sizeof(kSteps) / sizeof(kSteps[0])) {
        const Step& s = kSteps[step];
        Mission* m;
        if (s.type == 0 && s.station == 0 && s.param == 0) {
            m = new Mission();
        } else {
            m = new Mission(s.type, s.param, s.station);
        }
        if (s.campaign)
            setCampaignMission(m);
        else
            Status_addMissionTail(this, m);
    } else {
        // beyond the scripted range: default freelance hop.
        Status_addMissionTail(this, new Mission(0xb, 0, 100));
    }
}

// Same campaign-advance body, but takes (and ignores) a bool flag. The shipped
// binary loads this argument register but immediately overwrites it with the
// current campaign step before the dispatch switch, so the flag has no effect on
// the work performed; it simply forwards to the parameterless form.
void Status::nextCampaignMission(bool advance) {
    (void)advance;
    nextCampaignMission();
}

// Tail dispatch shared by addStationToStack()/visitStation(): commits `s` as the
// current station by routing through the full setStation() travel logic.
void Status::setStationTail(Station* s) {
    setStation(s);
}

// Travels to `s`: marks its system visible, then rebuilds the planet name and
// texture tables for every station in the destination system.
void Status::setStation(Station* s) {
    if (station == s) {
        return;
    }
    station = s;
    Galaxy* gal = gGalaxy;
    system = gal->getSystem(s->getSystem());
    if (system == 0) {
        return;
    }
    (*field_b4)[station->getSystem()] = true;
    FileRead fr;
    Array<Station*>* list = (Array<Station*>*)(intptr_t)fr.loadStationsBinary();
    if (planetNames != 0) {
        Array<String*>* pn = (Array<String*>*)(intptr_t)planetNames;
        for (auto* e : *pn) delete e;
        delete pn;
    }
    Array<String*>* names = new Array<String*>();
    planetNames = (int32_t)(intptr_t)names;
    names->resize(list->size());
    if (planetTextures != 0) {
        delete (Array<int>*)(intptr_t)planetTextures;
    }
    Array<int>* texs = new Array<int>();
    planetTextures = (int32_t)(intptr_t)texs;
    texs->resize(list->size());
    Array<Station*>* stations = (Array<Station*>*)asSystem(system)->getStations();
    for (unsigned i = 0; i < stations->size(); i = i + 1) {
        Station* cur = 0;
        bool found = false;
        for (unsigned j = 0; j < list->size(); j = j + 1) {
            cur = (*list)[j];
            int target = (int)(intptr_t)(*stations)[i];
            if (target == cur->getIndex()) {
                found = true;
                break;
            }
        }
        if (!found) {
            continue;
        }
        String* nm = new String();
        if (currentCampaignMission != 0) {
            *nm = cur->getName();
        }
        (*names)[i] = nm;
        (*texs)[i] = cur->getTextureIndex();
    }
    for (auto* e : *list) delete e;
    delete list;
}

// Status::replaceHash(haystack, needle, replacement) -> String
//   Replaces the first occurrence of `needle` in `haystack` with `replacement`.
//   If `needle` is absent, returns a copy of `haystack`.
String Status::replaceHash(String haystack, String needle, String replacement) {
    int idx = (int)haystack.IndexOf(needle);
    if (idx < 0) {
        return haystack;
    }

    String prefix;
    prefix.SubString(&haystack, 0, idx);

    String suffix;
    suffix.SubString(&haystack, needle.size() + idx, haystack.size());

    if (prefix.size() == 0) {
        return replacement + suffix;
    }
    return prefix + replacement + suffix;
}

// Status::replaceHash(haystack, needle) -> String
//   Forwards to the three-argument overload, substituting an empty replacement.
String Status::replaceHash(String haystack, String needle) {
    return replaceHash(haystack, needle, String(""));
}

// Adjusts the player rating by `delta`, clamping the result to [-10, 10].
void Status::changeRating(int delta) {
    int updated = delta + rating;
    rating = updated;
    if (updated < 0xb) {
        if (-0xb < updated) {
            return;
        }
        rating = -10;
    } else {
        rating = 10;
    }
}

extern int g_dsTypeMask;

// Status::departStation(Station* dest)
//   Leaving the current station to travel to `dest`: refreshes inventories, stacks
//   the prior station, regenerates the destination's wares/ships/agents, then
//   re-targets the active campaign/freelance mission, moves wanted criminals, and
//   clears per-cargo station amounts.
void Status::departStation(Station* dest) {
    // refresh the just-departed station inventory if it is the campaign hub in state 3.
    if (!inAlienOrbit()) {
        bool hub = station->getIndex() == 0x6c;
        if (hub && field_114 == 3) {
            Station* scratch = voidStation;
            scratch->setItems(station->getItems(), true);
            scratch->setShips(station->getShips(), true);
        }
    }

    bool wantedMove = false;
    if (!inAlienOrbit() && playerStation != dest) {
        wantedMove = dest->getIndex() != station->getIndex();
    }

    if (playerStation != dest) {
        bool wasOnStack = isOnStack(dest);
        Station* prev = isOnStack(dest) ? dest : 0;
        addStationToStack(dest);
        bool hub = dest->getIndex() == 0x6c;
        if (hub && field_114 == 3) {
            dest->setItems(dest->getItems(), true);
            dest->setShips(dest->getShips(), true);
            if (prev != 0 && prev != dest) {
                prev->setItems(prev->getItems(), true);
                prev->setShips(prev->getShips(), true);
            }
            if (!wasOnStack) {
                Generator g;
                dest->setAgents(g.createAgents(dest));
            }
        } else if (!wasOnStack) {
            Generator g;
            dest->setItems(g.getItemBuyList(dest), false);
            dest->setShips(g.getShipBuyList(dest), false);
            dest->setAgents(g.createAgents(dest));
        }
        field_70 = (int)playingTime;
        field_74 = (int)(playingTime >> 32);
    }

    // pick the active mission target for the new station.
    Status* st = gStatus;
    this->mission = 0;
    for (unsigned i = 0; i < missions->size(); i = i + 1) {
        Mission* mi = (*missions)[i];
        if (mi == 0)
            continue;
        int type = mi->getType();
        bool camp = mi->isCampaignMission();

        if (type == 0xad && camp) {
            int good = mi->getProductionGoodIndex();
            if (st->ship->hasCargo(good, 1)) {
                this->mission = mi;
                break;
            }
        }
        if (st->currentCampaignMission < 0x2d && type == 0xa1 && camp &&
            dest->getIndex() == field_80 && !inAlienOrbit()) {
            this->mission = mi;
            break;
        }
        if (!mi->isEmpty()) {
            int tgt = mi->getTargetStation();
            if (tgt == dest->getIndex() &&
                ((unsigned)(type - 0x96) > 0x17 || ((1 << ((type - 0x96) & 0xff)) & g_dsTypeMask) == 0) &&
                type != 8 && type != 0xe) {
                if (camp || (type != 0xb && type != 0xd)) { this->mission = mi; break; }
            }
        }
        if (!mi->isEmpty() && camp) {
            if (type == 0xa0 && mi->getTargetStation() != dest->getIndex()) {
                this->mission = mi;
                break;
            }
        }
    }

    // recompute the next freelance destination station.
    if (currentCampaignMission < 0x2d) {
        if (currentCampaignMission >= 0x20) {
            bool same = false;
            if (mission != 0)
                same = dest->getIndex() == mission->getTargetStation();
            same = same || dest->getIndex() == field_80;
            if (!same) {
                if (field_8c + 1 >= 10) {
                    field_8c = 0;
                    AbyssEngine::AERandom* rng = gRandom;
                    int sys;
                    do {
                        do {
                            sys = rng->nextInt(0x16);
                            field_7c = sys;
                        } while ((*systemVisibilities)[sys] == 0);
                    } while (sys == 10 || sys == 0xf);
                    SolarSystem* ss = gGalaxy->getSystems()->at(field_7c);
                    Array<int>* sids = (Array<int>*)ss->getStations();
                    int n = rng->nextInt(sids->size());
                    field_80 = (*sids)[n];
                } else {
                    field_8c = field_8c + 1;
                }
            }
        }
    } else {
        field_7c = -10;
        field_80 = -10;
    }

    if (wantedMove)
        moveWanted();

    // clear the per-cargo station-amount counters.
    Array<Item*>* cargo = ship->getCargo();
    if (cargo != 0) {
        for (unsigned i = 0; i < cargo->size(); i = i + 1) {
            Item* it = (*cargo)[i];
            if (it != 0)
                it->setStationAmount(0);
        }
    }
    field_0x108 = 0;
}

// Status::missionCompleted(bool atStation, bool docked, long long extra)
//   Scans the active mission list; for the first unfinished mission whose
//   completion criteria are met it returns that Mission (some types auto-mark
//   themselves won). Mission-type-specific checks mirror the engine's dispatch.
Mission* Status::missionCompleted(bool atStation, bool docked, long long extra) {
    unsigned which = (unsigned)docked;
    unsigned both = which & (unsigned)extra;

    for (unsigned i = 0; i < missions->size(); i = i + 1) {
        Mission* m = (*missions)[i];
        if (m->hasWon() != 0)
            return 0;

        // skip irrelevant missions (no agent/target/campaign tie-in).
        if (m == 0 ||
            (!m->isCampaignMission() && m->getAgent() == 0 &&
             m->getClientImage() == 0 && m->getTargetStation() == 0))
            continue;

        int type = m->getType();
        switch (type) {
        case 0x96:
            if (m->getStatusValue() <= missionCount) { m->setWon(true); return m; }
            break;
        case 0x97:
            if (m->getStatusValue() <= kills) { m->setWon(true); return m; }
            break;
        case 0x99:
            if (m->getStatusValue() <= stationsVisited) { m->setWon(true); return m; }
            break;
        case 0x9b:
            if (m->getStatusValue() <= goodsProduced) { m->setWon(true); return m; }
            break;
        case 0x98: {
            Array<Item*>* eq = ship->getEquipment();
            for (unsigned j = 0; j < eq->size(); j = j + 1) {
                Item* it = (*eq)[j];
                if (it != 0 && it->getIndex() == m->getStatusValue())
                    return m;
            }
            break;
        }
        case 0x9a:
            if (m->getProductionGoodAmount() <= ship->getCurrentLoad()) { m->setWon(true); return m; }
            break;
        case 0xa7:
        case 0xae:
            if (m->getProductionGoodAmount() <= m->getStatusValue()) { m->setWon(true); return m; }
            break;
        case 0x9c:
            if (!docked) {
                if (extra >= 0x2711 && station->getIndex() == m->getTargetStation()) return m;
            }
            break;
        case 0xa0:
            if (docked) return m;
            if (extra < 0x2711 && station->getIndex() != m->getTargetStation()) return m;
            break;
        case 0x9d: {
            Array<Item*>* eq = ship->getEquipment();
            for (unsigned j = 0; j < eq->size(); j = j + 1) {
                Item* it = (*eq)[j];
                if (it != 0 && it->getType() == m->getStatusValue())
                    return m;
            }
            break;
        }
        case 0x9e: {
            Array<Item*>* eq = ship->getEquipment();
            bool hasGood = false, hasSpecial = false;
            for (unsigned j = 0; j < eq->size(); j = j + 1) {
                Item* it = (*eq)[j];
                if (it != 0) {
                    if (it->getType() == 0)
                        hasGood = true;
                    else if (it->getSort() == 10)
                        hasSpecial = true;
                }
            }
            if (hasGood && hasSpecial) return m;
            break;
        }
        case 0xa2:
            if (docked && station->getIndex() == m->getTargetStation()) {
                BluePrint* bp = (*bluePrints)[m->getStatusValue()];
                if (bp->getCompletionRate() >= 1.0f) return m;
            }
            break;
        case 0xa3: {
            Array<int>* arr = field_90;
            bool all = true;
            for (unsigned j = 0; j < arr->size(); j = j + 1) {
                if ((*arr)[j] < 0) { all = false; break; }
            }
            if (all) return m;
            break;
        }
        case 0xa8: {
            Status* s = gStatus;
            if (m->getStatusValue() <= s->field_178) {
                m->setWon(true);
                s->field_178 = 0;
                return m;
            }
            break;
        }
        case 0xaa:
            if (m->getStatusValue() == 1) { m->setWon(true); return m; }
            break;
        case 0xab:
            if (docked && station->getIndex() == m->getTargetStation()) return m;
            break;
        case 0xac:
            if (both != 0 && station->getIndex() == m->getTargetStation()) {
                int idx = m->getProductionGoodIndex();
                int amt = m->getProductionGoodAmount();
                if (Item::isInList(idx, amt, ship->getCargo())) return m;
            }
            break;
        case 0xa6:
            if (docked && station->getIndex() == m->getTargetStation()) {
                int idx = m->getProductionGoodIndex();
                int amt = m->getProductionGoodAmount();
                if (Item::isInList(idx, amt, ship->getCargo())) return m;
                if (!gStatus->ship->hasEquipment(m->getProductionGoodIndex(), 1)) return m;
            }
            break;
        case 0xb8:
            if (m->getStatusValue() == 0) {
                Status* s = gStatus;
                if (s->currentCampaignMission != 0x5c || s->inAlienOrbit() ||
                    s->station->getIndex() != 0x71) {
                    m->setWon(true);
                    return m;
                }
            }
            break;
        case 0xbd:
            if (docked) {
                Array<Item*>* eq = ship->getEquipment();
                for (unsigned j = 0; j < eq->size(); j = j + 1) {
                    Item* it = (*eq)[j];
                    if (it != 0 && it->getSort() == m->getStatusValue())
                        return m;
                }
            }
            break;
        default:
            // legacy mission ids 8..0xe.
            if (type == 8) {
                // Campaign pending-product hand-in special case (binary @0x000bc460):
                // when on the campaign mission 0x8f and not yet docked, look for a
                // pending product whose word @+0x14 == 0xd2 at the target station.
                if (m->isCampaignMission() && gStatus->currentCampaignMission == 0x8f && !docked) {
                    if (station->getIndex() == m->getTargetStation() && pendingProducts != 0) {
                        for (unsigned j = 0; j < pendingProducts->size(); j = j + 1) {
                            PendingProduct* pp = (*pendingProducts)[j];
                            if (pp != 0 && i32(pp, 0x14) == 0xd2)
                                return m;
                        }
                    }
                }
                if (docked && station->getIndex() == m->getTargetStation()) {
                    int idx = m->getProductionGoodIndex();
                    int amt = m->getProductionGoodAmount();
                    if (Item::isInList(idx, amt, ship->getCargo())) return m;
                }
            } else if (type == 0xd) {
                if (docked && field_0xf0 != 0) return m;
            } else if (type == 0xe) {
                if (docked) {
                    Agent* ag = m->getAgent();
                    if (station->getIndex() == ag->getStation()) {
                        if (Item::isInList(0x73, ship->getCargo())) return m;
                    }
                }
            } else if (type == 0xb || type == 0) {
                if (docked && station->getIndex() == m->getTargetStation()) return m;
            }
            break;
        }
    }
    return 0;
}

// Reads the agent roster from disk via a transient FileRead helper.
void Status::loadAgents() {
    FileRead fr;
    agents = fr.loadAgents();
}

// Identity passthrough variant: hands back the agent list it was given unchanged.
Array<int>* Status::loadAgents(Array<int>* agents) {
    return agents;
}

extern int g_levelXPTable[];

// Recomputes the player level from the weighted experience sum against the XP
// threshold table.
void Status::checkForLevelUp() {
    int d4 = field_d4 / 3;
    int a0d = field_a0 / 0x32;
    int sum = a0d + (kills + d4) + field_a4 + missionCount * 2 + currentCampaignMission + stationsVisited;
    for (int i = 0; i != 0x15; i = i + 1) {
        if (sum >= g_levelXPTable[i]) {
            level = i;
        }
    }
}

extern Station** g_mwPrevStation;

// Status::moveWanted()
//   Advances each active, non-terminated wanted criminal one hop along a pathfound
//   route toward its destination (subject to system-visibility and forbidden-system
//   rules), updating its current location / travels-to / last-seen state.
void Status::moveWanted() {
    Array<SolarSystem*>* systemsTable = 0;
    SystemPathFinder* pf = 0;
    Station** prevHolder = g_mwPrevStation;

    for (unsigned i = 0; i < wanted->size(); i = i + 1) {
        Wanted* w = (*wanted)[i];
        if (w->isActive() == 0)
            continue;
        if (w->isTerminated() != 0)
            continue;
        if (inAlienOrbit())
            continue;

        if (w->getCurrentLocation() == station->getIndex())
            continue;
        if (*prevHolder != 0) {
            if ((*prevHolder)->getIndex() == w->getCurrentLocation())
                continue;
        }

        // lazily load the systems table + pathfinder on first need.
        if (pf == 0) {
            FileRead fr;
            systemsTable = fr.loadSystemsBinary();
            pf = new SystemPathFinder();
        }

        // resolve the from/to stations.
        Station* fromSt = gGlobals->getRandomStation();
        int fromSys = fromSt->getSystem();
        Station* toSt = gGlobals->getRandomStation();
        int toSys = toSt->getSystem();

        Array<int>* path = 0;
        if (fromSt->getIndex() == toSt->getIndex()) {
            // already at destination system: re-roll a random reachable target.
            unsigned lo, hi;
            if (i < 2) { hi = 4; lo = 2; }
            else { int r = (i - 1) % 6; lo = r / 3 + 2; hi = r / 2 + 4; }
            w->setLastSeen(w->getCurrentLocation());
            if (toSt != 0) delete toSt;
            toSt = gGlobals->getRandomStation();
            toSys = toSt->getSystem();
            for (;;) {
                path = pf->getSystemPath(systemsTable, fromSys, toSys);
                int dsys = toSt->getSystem();
                int routes = (int)(intptr_t)(*systemsTable)[dsys]->getRoutes();
                bool ok = path != 0 && routes != 0 &&
                          (unsigned)(*path)[0] >= lo && (unsigned)(*path)[0] <= hi &&
                          (*systemVisibilities)[dsys] != 0 &&
                          dsys != 0x1b && dsys != 0x1c && dsys != 0x19 && dsys != 0x1a && dsys != 6 &&
                          dsys != fromSt->getSystem();
                if (ok) {
                    w->setTravelsTo(toSt->getIndex());
                    break;
                }
                if (toSt != 0) delete toSt;
                toSt = gGlobals->getRandomStation();
                toSys = toSt->getSystem();
            }
        } else if (fromSys == toSys) {
            w->setCurrentLocation(w->getTravelsTo());
            path = 0;
        } else {
            path = pf->getSystemPath(systemsTable, fromSys, toSys);
            int wg = (*systemsTable)[(*path)[1]]->getWarpGateIndex();
            w->setCurrentLocation(wg);
        }

        if (toSt != 0) delete toSt;
        if (fromSt != 0) delete fromSt;
        if (path != 0) delete path;
    }

    if (systemsTable != 0) {
        for (auto* e : *systemsTable) delete e;
        delete systemsTable;
    }
    if (pf != 0) {
        delete pf;
    }
}

// Replaces the wingmen roster with deep copies of the strings in `list` (or clears it).
void Status::setWingmen(Array<String*>* list) {
    Array<String*>* cur = (Array<String*>*)(intptr_t)wingmen;
    if (cur != 0) {
        for (auto* e : *cur) delete e;
        cur->clear();
    }
    if (list == 0) {
        wingmen = 0;
        this->field_0x28 = 0;
        this->field_0x30 = 0;
    } else {
        Array<String*>* na = new Array<String*>();
        wingmen = (int32_t)(intptr_t)na;
        na->resize(list->size());
        for (unsigned i = 0; i < list->size(); i = i + 1) {
            String* s = new String();
            s->copy((*list)[i], false);
            (*na)[i] = s;
        }
    }
}
