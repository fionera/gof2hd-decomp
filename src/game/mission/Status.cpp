#include "gof2/game/mission/Status.h"
#include "gof2/game/mission/PendingProduct.h"
#include "gof2/engine/core/AERandom.h"
#include "gof2/externs.h"
// FileRead.h is intentionally NOT included: it defines layout-stub structs
// (Station, Item, SolarSystem, Agent, Wanted, Ship, ...) that collide with the
// real class headers included below. This TU only needs FileRead itself, which
// is declared locally (see `struct FileRead` further down) plus the extern "C"
// FileRead_* entry points.
struct FileRead;
#include "gof2/game/world/Galaxy.h"
#include "gof2/game/mission/Item.h"
// SystemPathFinder.h is intentionally NOT included: it defines layout-stub
// structs Status and SolarSystem that collide with the real Status.h/SolarSystem.h.
// The only SystemPathFinder entry point used here is getSystemPath; it is declared
// locally below with the decompiler-observed ABI (int* path, opaque void* systems).
#include "gof2/game/mission/Achievements.h"
// Agent.h, Station.h, SolarSystem.h and Wanted.h each define an identical,
// layout-compatible `struct RetStr` at global scope. Including more than one in a
// single TU is a C++ redefinition. Let Agent.h own the canonical RetStr and rename
// the duplicates from the other three headers (Station.h is pulled in transitively by
// Mission.h). Their renamed RetStr is unused here: the only RetStr-returning call,
// Station::getName(), discards its result, so the ABI is unaffected.
#include "gof2/game/ship/Agent.h"
#include "gof2/game/mission/BluePrint.h"
#define RetStr RetStr
#include "gof2/game/mission/Mission.h"   // pulls in Station.h
#include "gof2/game/world/Station.h"
#undef RetStr
#define RetStr RetStr
#include "gof2/game/world/SolarSystem.h"
#undef RetStr
#define RetStr RetStr
#include "gof2/game/world/Wanted.h"
#include "gof2/game/ship/Ship.h"
#undef RetStr

extern "C" int AEString_IndexOf(String *haystack, String *needle);
extern "C" void *Ship_dtor(Ship *);
extern "C" void op_delete(void *);
extern "C" int Station_getIndex(Station *s);
extern "C" void *Ship_dtor(Ship *s);
extern "C" void ArrayReleaseClasses_Agent(Array<Agent *> *a);
extern "C" void *Array_Agent_dtor(Array<Agent *> *a);
extern "C" void ArrayReleaseClasses_Station(void *a);
extern "C" void *Array_Station_dtor(Array<Station *> *a);
extern "C" void ArrayReleaseClasses_Wanted(Array<Wanted *> *a);
extern "C" void *Array_Wanted_dtor(Array<Wanted *> *a);
extern "C" void incKills_notify(void *arg);
extern "C" int Station_getIndex(Station *);
extern "C" void incPirateKills_notify(void *arg);
extern "C" void Array_Mission_ctor(Array<Mission *> *a);
extern "C" void Array_Station_ctor(Array<Station *> *a);
extern "C" void Array_bool_ctor(void *a);
extern "C" void Array_int_ctor(void *a);
extern "C" void ArraySetLength_Mission(uint32_t n, Array<Mission *> *a);
extern "C" void ArraySetLength_Station(uint32_t n, Array<Station *> *a);
extern "C" void ArraySetLength_bool(int len, void *a);
extern "C" bool Status_inAlienOrbit_impl(Station *station, Station *playerStation);
extern "C" int __aeabi_idiv(int a, int b);
extern "C" int __aeabi_idivmod(int a, int b);
extern "C" void SystemPathFinder_ctor(SystemPathFinder *p);
extern "C" void *SystemPathFinder_dtor(SystemPathFinder *p);
extern "C" void FileRead_ctor(FileRead *fr);
extern "C" void *FileRead_dtor(FileRead *fr);
Station *Globals_getRandomStation();
extern "C" int Station_getSystem(Station *s);
extern "C" void *Array_int_dtor(void *a);
extern "C" void ArrayReleaseClasses_SolarSystem(void *a);
extern "C" void *Array_SolarSystem_dtor(void *a);
extern "C" void Array_PendingProduct_ctor(int *a);
extern "C" void PendingProduct_ctor(int *pp, BluePrint *bp);
extern "C" void FileRead_ctor(FileRead *);
extern "C" void operator_delete_tail(void *);
extern "C" void ArrayReleaseClasses_String(void *a);
extern "C" void *Array_String_dtor(Array<String *> *a);
extern "C" void Array_String_ctor(Array<String *> *a);
extern "C" void ArraySetLength_String(uint32_t n, Array<String *> *a);
extern "C" void ArrayRelease_int(void *a);
extern "C" void ArraySetLength_int(int len, void *a);
extern "C" void String_ctor_empty(String *s);
extern "C" int Station_getTextureIndex(Station *s);
extern "C" void ArrayReleaseClasses_Station(void *a);
extern "C" void Array_Station_dtor_tail(void *a);

// Local engine helper types used across several member functions in this translation unit.
// Defined once here; the per-function blocks below merely forward-declare them.
struct FileRead {
    FileRead();
    ~FileRead();
    int loadStationsBinary();
    Array<SolarSystem *> *loadSystemsBinary();
    Array<Wanted *> *loadWanted();
    Array<Agent *> *loadAgents();
};
struct Generator { Generator(); };

int Status::getPlanetNames() { return planetNames; }

int Status::getJumpgateUsed() { return jumpgatesUsed; }

bool Status::dlc1Won() { return 0x53 < currentCampaignMission; }

void Status::incEquipmentBought() { boughtEquipment = boughtEquipment + 1; }

void Status::setKills(int v) { kills = v; }

void Status::incMissionCount() { missionCount = missionCount + 1; }

void Status::incPlayingTime(int64_t delta) { playingTime = playingTime + delta; }

uint Status::orbitHasPlanetRing(int index) {
    unsigned t = index - 0x78U;
    if (((t >> 1) | (t << 0x1f)) < 7) {
        return 0x69U >> ((index - 0x78U >> 1) & 0x7f) & 1;
    }
    return 0;
}

int Status::getSystem() { return system; }

// Returns 1 only when stranded in the supernova solar system before the late campaign.
int Status::inSupernovaSystem() {
    if (__builtin_expect((long)inAlienOrbit(), 1)) return 0;
    int idx = ((SolarSystem *)(system))->getIndex();
    int result = 0;
    if (idx == 0x1b && currentCampaignMission < 0x9e) result = 1;
    return result;
}

// ---- Status::isChallengeMode() ----
// Static HUD predicate (Status_isChallengeMode, called from Hud::draw with no receiver):
// the game's only "challenge mode" is the supernova challenge, so this resolves the live
// status from the global board and reports whether the player is currently stranded in the
// supernova system (the same condition that gates Hud::drawChallengeModeScore). The board
// slot is the same singleton the other no-receiver Status queries (e.g. wantedBoardAccessible)
// read from; if it is not yet wired up there is no challenge in progress.
__attribute__((visibility("hidden"))) extern Status **g_statusBoard;

int Status::isChallengeMode() {
    Status **slot = g_statusBoard;
    if (slot == 0 || *slot == 0) return 0;
    return (*slot)->inSupernovaSystem();
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

void Status::setPassengers(int p) { passengers = p; }

int Status::getMissionCount() { return missionCount; }

void Status::setCredits(int v) { credits = v; }

bool Status::stringHasToken(String haystack, String needle) {
    return AEString_IndexOf(&haystack, &needle) > -1;
}

int Status::getLastXP() { return lastXP; }

Array<int> *Status_loadAgents_passthrough(Array<int> *a) {
    return a;
}

void Status::setLastXP(int v) { lastXP = v; }

void Status::setStationsVisited(int v) { stationsVisited = v; }

void Status::setStationStack(Array<Station *> *stack) { stationStack = stack; }

struct HardCoreHolder { char pad[0x2c]; float difficulty; };
__attribute__((visibility("hidden"))) extern HardCoreHolder *g_hardCoreHolder;
bool Status::hardCoreMode() {
    return g_hardCoreHolder->difficulty == 1.5f;
}

void Status::crateCaptured(int delta) { capturedCrates = delta + capturedCrates; }

int Status::getStanding() { return standing; }

int Status::getBoughtEquipment() { return boughtEquipment; }

int Status::getCredits() { return credits; }

void Status::setRating(int v) { rating = v; }

Array<Wanted *> *Status::getWanted() { return wanted; }

int Status::getCurrentCampaignMission() { return currentCampaignMission; }

void Status::setMissionCount(int v) { missionCount = v; }

void Status::setShip(Ship *s) {
    if (ship != 0) {
        op_delete(Ship_dtor(ship));
        ship = 0;
    }
    ship = s;
}

Array<Mission *> *Status::getMissions() { return missions; }

bool Status::inFogSkyboxOrbit() {
    if (inAlienOrbit()) return false;
    if (((SolarSystem *)(system))->getTextureIndex() == 0x11) return true;
    return ((SolarSystem *)(system))->getTextureIndex() == 0x12;
}

bool Status::inPirateLootOrbit() {
    if (inAlienOrbit()) return false;
    if (((SolarSystem *)(system))->getIndex() == 0x20) return true;
    return ((SolarSystem *)(system))->getIndex() == 0x21;
}

void Status::setPlayingTime(int64_t v) { playingTime = v; }

int Status::getPendingProducts() { return (int)(intptr_t)pendingProducts; }

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
    ((Mission *)(m))->setCampaignMission(true);
    Mission **slot = missions->data();
    int *cur = (int *)slot[0];
    if (cur != 0 && cur != g_campaignSentinel) {
        (*(void (**)(int *))(*cur + 4))(cur);
        slot[0] = 0;
    }
    slot[0] = m;
}

// Returns the active, non-terminated wanted target located in the current orbit with the
// lowest required-bounty count.
Wanted *Status::getWantedInCurrentOrbit() {
    Array<Wanted *> *arr = wanted;
    Wanted *best = 0;
    if (arr != 0) {
        unsigned i = 0;
        while (i < arr->size()) {
            if (((Wanted *)((*wanted)[i]))->isActive() != 0 &&
                ((Wanted *)((*wanted)[i]))->isTerminated() == 0) {
                int loc = ((Wanted *)((*wanted)[i]))->getCurrentLocation();
                if (loc == Station_getIndex(station)) {
                    if (best == 0 ||
                        ((Wanted *)(best))->getRequiredBounties() <
                            ((Wanted *)((*wanted)[i]))->getRequiredBounties()) {
                        best = (*wanted)[i];
                    }
                }
            }
            i = i + 1;
            arr = wanted;
        }
    }
    return best;
}

int Status::getLevel() { return level; }

// Releases the ship, campaign mission, agent/station-stack/wanted arrays, and the name string.
Status::~Status() {
    if (ship != 0) {
        ::operator delete(Ship_dtor(ship));
    }
    ship = 0;
    if (mission != 0) {
        (*(void (**)(Mission *))(*(int *)mission + 4))(mission);
    }
    mission = 0;
    if (agents != 0) {
        ArrayReleaseClasses_Agent(agents);
        if (agents != 0) {
            ::operator delete(Array_Agent_dtor(agents));
        }
    }
    agents = 0;
    if (stationStack != 0) {
        ArrayReleaseClasses_Station(stationStack);
        if (stationStack != 0) {
            ::operator delete(Array_Station_dtor(stationStack));
        }
    }
    stationStack = 0;
    if (wanted != 0) {
        ArrayReleaseClasses_Wanted(wanted);
        if (wanted != 0) {
            ::operator delete(Array_Wanted_dtor(wanted));
        }
    }
    wanted = 0;
}

bool Status::inStormOrbit() {
    if (__builtin_expect((long)inAlienOrbit(), 0)) return false;
    if (__builtin_expect(currentCampaignMission < 0x5a, 0)) return false;
    if (inSupernovaSystem() != 0) return true;
    if (((SolarSystem *)(system))->getTextureIndex() == 0x10) return true;
    return ((SolarSystem *)(system))->getTextureIndex() == 0x12;
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
// entry of the missions array is the active campaign mission. MGame uses this to
// reach the mission's agent, dialogue, type and status fields.
Mission *Status::getCampaignMissionPtr() {
    return (*missions)[0];
}

void Status::setJumpgateUsed(int v) { jumpgatesUsed = v; }

bool Status::gameWon() { return 0x2c < currentCampaignMission; }

void Status::setLevel(int v) { level = v; }

void Status::setCapturedCrates(int v) { capturedCrates = v; }

bool Status::inBlackMarketSystem() {
    if (inAlienOrbit()) return false;
    return ((SolarSystem *)(system))->getIndex() == 0x19;
}

void Status::incGoodsProduced(int delta) { goodsProduced = delta + goodsProduced; }

int64_t Status::getPlayingTime() { return playingTime; }

// Counts the non-empty slots in the mission array.
int Status::getNumberOfMissions() {
    Array<Mission *> *m = missions;
    if (m == 0) {
        return 0;
    }
    int count = 0;
    int i = 0;
    while ((int)m->size() != i) {
        Mission *cur = (*m)[i];
        i = i + 1;
        if (cur != 0) {
            count = count + 1;
        }
    }
    return count;
}

bool Status::inSupernovaOrbit() {
    if (inAlienOrbit()) return false;
    return Station_getIndex(station) == 0x6d;
}

bool Status::inDeepScienceOrbit() {
    if (inAlienOrbit()) return false;
    if (Station_getIndex(station) == 10) return true;
    return Station_getIndex(station) == 100;
}

__attribute__((visibility("hidden"))) extern Status **g_statusBoard;

// True if any wanted entry can post on the current system's bounty board right now.
int Status::wantedBoardAccessible() {
    Status **slot = g_statusBoard;
    Array<Wanted *> *w = (*slot)->wanted;
    for (unsigned i = 0; i < w->size(); i = i + 1) {
        int race = ((SolarSystem *)((*slot)->system))->getRace();
        int board = ((Wanted *)((*w)[i]))->getBoard();
        if (race == board) {
            int ccm = (*slot)->currentCampaignMission;
            if (ccm >= ((Wanted *)((*w)[i]))->getRequiredMission() &&
                (*slot)->inAlienOrbit() == 0 &&
                Station_getIndex((*slot)->station) != 0x6c) {
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

int Status::getPassengers() { return passengers; }

bool Status::isStorylineWanted(int index) { return (unsigned)index < 2; }

// Returns the first failed mission, or a type-0xd mission flagged failable (byte at 0xf1).
int Status::missionFailed(bool param_1, int64_t time) {
    for (unsigned i = 0; i < missions->size(); i = i + 1) {
        Mission *cur = (*missions)[i];
        if (((Mission *)(cur))->hasFailed() != 0) {
            return 0;
        }
        if (cur != 0 && ((Mission *)(cur))->getType() == 0xd && param_1 &&
            *((uint8_t *)this + 0xf1) != 0) {
            return (int)(intptr_t)cur;
        }
    }
    return 0;
}

int Status::getBluePrints() { return (int)(intptr_t)bluePrints; }

int Status::getAgents() { return (int)(intptr_t)agents; }

uint Status::inPlanetRingOrbit() {
    if (inAlienOrbit() == 0) {
        int idx = Station_getIndex(station);
        unsigned t = idx - 0x78U;
        if (((t >> 1) | (t << 0x1f)) < 7) {
            return 0x69U >> ((idx - 0x78U >> 1) & 0x7f) & 1;
        }
    }
    return 0;
}

void Status::setBoughtEquipment(int v) { boughtEquipment = v; }

int Status::getWingmen() { return wingmen; }

int Status::getGoodsProduced() { return goodsProduced; }

int Status::getKills() { return kills; }

// Credit deltas whose magnitude exceeds 1e9 are ignored; the balance is clamped at >= 0.
__attribute__((minsize)) void Status::changeCredits(int delta) {
    int magnitude = delta;
    if (delta < 0) {
        magnitude = -delta;
    }
    if (magnitude <= 1000000000) {
        unsigned credited = (unsigned)(delta + credits);
        credits = credited & ~((int)credited >> 0x1f);
    }
}

// True when `station` hosts a freighter mission (two bit-masks plus the special id 0xf).
__attribute__((visibility("hidden"))) extern int DAT_freighterA;
__attribute__((visibility("hidden"))) extern int DAT_freighterB;

int Status::isFreighterMissionStation(int station) {
    if ((unsigned)(station - 0x1e) < 0x1f && ((1 << ((station - 0x1e) & 0xff)) & DAT_freighterA) != 0)
        return 1;
    if ((unsigned)(station - 0x46) < 0x1a && ((1 << ((station - 0x46) & 0xff)) & DAT_freighterB) != 0)
        return 1;
    if (station == 0xf) return 1;
    return 0;
}

// Returns whether the blueprint with item index `index` has been unlocked.
int Status::isBlueprintUnlocked(int index) {
    Array<BluePrint *> *bps = bluePrints;
    unsigned i = 0;
    while (true) {
        if (bps->size() <= i) {
            return 0;
        }
        if (((BluePrint *)((*bps)[i]))->getIndex() == index) {
            break;
        }
        i = i + 1;
    }
    return ((BluePrint *)((*bluePrints)[i]))->isUnlocked();
}

// Allocates the mission/station/visibility containers, sizes them, and zero-initializes the
// persistent player state to its new-game defaults.
Status::Status() {
    Array<Mission *> *m = (Array<Mission *> *)operator new(0xc);
    Array_Mission_ctor(m);
    missions = m;
    Array<Station *> *ss = (Array<Station *> *)operator new(0xc);
    Array_Station_ctor(ss);
    stationStack = ss;
    Array<bool> *a;
    a = (Array<bool> *)operator new(0xc); Array_bool_ctor(a); systemVisibilities = a;
    a = (Array<bool> *)operator new(0xc); Array_bool_ctor(a); field_94 = a;
    a = (Array<bool> *)operator new(0xc); Array_bool_ctor(a); field_98 = a;
    a = (Array<bool> *)operator new(0xc); Array_bool_ctor(a); field_ac = a;
    a = (Array<bool> *)operator new(0xc); Array_bool_ctor(a); field_b4 = a;
    a = (Array<bool> *)operator new(0xc); Array_bool_ctor(a); field_4c = a;
    a = (Array<bool> *)operator new(0xc); Array_bool_ctor(a); field_50 = a;
    void *ai = operator new(0xc); Array_int_ctor(ai); field_90 = ai;
    a = (Array<bool> *)operator new(0xc); Array_bool_ctor(a); field_54 = a;
    a = (Array<bool> *)operator new(0xc); Array_bool_ctor(a); field_58 = a;
    ArraySetLength_Mission(2, missions);
    ArraySetLength_Station(3, stationStack);
    ArraySetLength_bool(0x22, systemVisibilities);
    ArraySetLength_bool(0xb, field_94);
    ArraySetLength_bool(0xb, field_98);
    ArraySetLength_bool(0x16, field_ac);
    ArraySetLength_bool(0x22, field_b4);
    ArraySetLength_bool(4, field_4c);
    ArraySetLength_bool(0xf, field_50);
    ArraySetLength_bool(0xe9, field_54);
    ArraySetLength_bool(5, field_58);
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
    Station *st = (Station *)operator new(0x34);
    ((Station *)(st))->ctor_default();
    playerStation = st;
    field_8c = 0;
    ship = 0;
    mission = 0;
    station = 0;
    system = 0;
    planetNames = 0;
    planetTextures = 0;
    field_14c = 0;
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
    for (int i = 1; i != 5; i = i + 1) {
        *(int32_t *)((char *)this + i * 4) = 0;
    }
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

struct Galaxy;
__attribute__((visibility("hidden"))) extern Galaxy **g_ccpGalaxy;
__attribute__((visibility("hidden"))) extern int *g_ccpRandom;
__attribute__((visibility("hidden"))) extern int *g_ccpPrice;        // DAT_000bccc0 / extra
__attribute__((visibility("hidden"))) extern int **g_ccpPriceMod;

extern "C" {
int Ship_getEquipment2(Ship *ship);
int Ship_getCargo2(void);
int Station_getItems2(void);
int Station_getIndex2(Station *s);
int Status_inAlienOrbit2(void);
int SolarSystem_getIndex2(int sys);
// 0x73864-area accessor 1
int AERandom_nextInt2(int rng, int bound);
void Item_setPrice2(void *item, int price);
void AERandom_reset2(int rng);
int Item_coordA(int sysCoord);                                       // 0x73a... (blx r9)  coord accessor
int Item_coordB(int sysCoord);                                       // (blx r10) coord accessor
int Item_getIndex2(void *item);                                      // (blx r11)
}

// Status::calcCargoPrices()
//   Reprices every item in the player's cargo, equipment, and current station inventory based
//   on the galactic distance between the item's cheapest/most-expensive systems and the current
//   system, clamped to the item's price band with a small per-system random jitter.
void Status::calcCargoPrices()
{
    Galaxy *gal = *g_ccpGalaxy;
    int systems = (int)(intptr_t)((Galaxy *)(gal))->getSystems();
    int *rng = g_ccpRandom;
    const float kPriceScale = 1.0f;
    const float kJitter = 1.0f;

    for (unsigned src = 0; src != 3; src = src + 1) {
        unsigned which = src & 0x7fffffff;
        unsigned *list;
        if (which == 1)
            list = (unsigned *)Ship_getEquipment2(this->ship);
        else if (which == 0)
            list = (unsigned *)Ship_getCargo2();
        else
            list = (unsigned *)Station_getItems2();
        if (list == 0)
            continue;

        // seed by the station index so prices are deterministic per visit.
        ((AbyssEngine::AERandom *)(*rng))->setSeed((long long)Station_getIndex2(this->station));
        bool ringWorld = false;
        if (Status_inAlienOrbit2() == 0)
            ringWorld = (SolarSystem_getIndex2(this->system) == 0x19);

        unsigned n = *list;
        for (unsigned i = 0; i < n; i = i + 1) {
            void *item = *(void **)(list[1] + i * 4);
            if (item == 0)
                continue;

            // distance percent for the item's min-price system pair.
            int aMin = Item_coordA(*(int *)(*(int *)(systems + 4) + Item_getIndex2(item) * 4));
            int aMax = Item_coordB(*(int *)(*(int *)(systems + 4) + Item_getIndex2(item) * 4));
            int bMin = Item_coordA(*(int *)(*(int *)(systems + 4) + ((Item *)(item))->getMaxPriceSystem() * 4));
            int bMax = Item_coordB(*(int *)(*(int *)(systems + 4) + ((Item *)(item))->getMaxPriceSystem() * 4));
            int dItem = ((Galaxy *)(gal))->distancePercent(aMin, aMax, bMin, bMax);

            // distance percent for the current station's system.
            int cMin = Item_coordA(*(int *)(*(int *)(systems + 4) + Item_getIndex2(item) * 4));
            int cMax = Item_coordB(*(int *)(*(int *)(systems + 4) + Item_getIndex2(item) * 4));
            int sSys = Station_getIndex2(this->station);
            int sMin = Item_coordA(*(int *)(*(int *)(systems + 4) + sSys * 4));
            int sMax = Item_coordB(*(int *)(*(int *)(systems + 4) + sSys * 4));
            int dHere = ((Galaxy *)(gal))->distancePercent(cMin, cMax, sMin, sMax);

            float t = ((kPriceScale / (float)dItem) * (float)dHere) / kPriceScale;
            int minPrice = ((Item *)(item))->getMinPrice();
            int band = ((Item *)(item))->getMaxPrice() - ((Item *)(item))->getMinPrice();
            float clamp = t < 1.0f ? t : 1.0f;

            if (((Item *)(item))->getSinglePrice() > 0) {
                int price;
                if (ringWorld) {
                    price = ((Item *)(item))->getMaxPrice();
                } else {
                    int base = minPrice + (int)(clamp * (float)band);
                    int jitterMax = (int)((float)base * kJitter);
                    if (jitterMax < 2)
                        jitterMax = 1;
                    price = (base - jitterMax) + AERandom_nextInt2(*rng, jitterMax | 1);
                }
                if (*g_ccpPriceMod != 0 && **g_ccpPriceMod != 0) {
                    // apply a global price modifier when present.
                    price = (int)((float)price + (float)price * (float)(**g_ccpPriceMod));
                }
                Item_setPrice2(item, price);
            }
        }
        AERandom_reset2(*rng);
    }
}

// Returns the matching stack slot's station (truthy) if `s` is already on the 3-deep
// station stack, else 0.  The original keeps this as a rolled loop over slots 0..2.
bool Status::isOnStack(Station *s) {
    unsigned i = 0;
    while (true) {
        if (2 < i) {
            return 0;
        }
        Station *cur = (*stationStack)[i];
        if (cur != 0 && ((Station *)(cur))->equals(s) != 0) {
            break;
        }
        i = i + 1;
    }
    return (*stationStack)[i] != 0;
}

// True iff the orbited station differs from the player's home station. departStation()
// uses the same `playerStation != dest` test, which fixes the polarity here.
bool Status::inAlienOrbit_impl(Station *station, Station *playerStation) {
    return station != playerStation;
}

bool Status::inAlienOrbit() {
    return Status_inAlienOrbit_impl(station, playerStation);
}

// Local offset-cast helpers (the Status header models fields by name, but resetGame
// touches many raw offsets so byte-offset access is clearer here).
// I(void*,int), P(void*,int) and C(void*,int) are provided by gof2/Galaxy.h /
// common.h; no local redefinition needed here.

struct Standing;
struct Galaxy;
struct PendingProduct;
struct Achievements;

// PC-relative singleton / global slots referenced by resetGame.
__attribute__((visibility("hidden"))) extern int   *g_rg_settings;       // [DAT_000bd694] -> settings record
__attribute__((visibility("hidden"))) extern Galaxy **g_rg_galaxy;       // [DAT_000bd69c] -> Galaxy*
__attribute__((visibility("hidden"))) extern int  **g_rg_itemTable;      // [DAT_000bd980] -> Array<Item*>*
__attribute__((visibility("hidden"))) extern Achievements **g_rg_ach;    // [DAT_000bd6a0]
__attribute__((visibility("hidden"))) extern int  **g_rg_statusSlotA;    // [DAT_000bd994]
__attribute__((visibility("hidden"))) extern int  **g_rg_statusSlotB;    // [DAT_000bd988]
__attribute__((visibility("hidden"))) extern int  **g_rg_statusSlotC;    // [DAT_000bd984]
__attribute__((visibility("hidden"))) extern int  **g_rg_zeroSlotA;      // [DAT_000bd6a4]
__attribute__((visibility("hidden"))) extern char **g_rg_zeroSlotB;      // [DAT_000bd6a8]
__attribute__((visibility("hidden"))) extern int  **g_rg_zeroSlotC;      // [DAT_000bd6ac]
__attribute__((visibility("hidden"))) extern int (*g_rg_makeItemB)(int); // [DAT_000bd98c]
__attribute__((visibility("hidden"))) extern void (*g_rg_addCargo)(int, int, int); // [DAT_000bd990]

extern "C" {
void  *Status_opnew(unsigned int size);
void  *ArrayString_dtor(void *a);
void   ArrayReleaseClasses_String(void *a);
void  *Array_int_dtor(void *a);
void   Array_int_ctor(void *a);
void   ArraySetLength_int(int len, void *a);
void  *Array_bool_dtor(void *a);
void   Array_bool_ctor(void *a);
void   ArraySetLength_bool(int len, void *a);
void  *ArrayBP_dtor(void *a);
void   ArrayBP_ctor(void *a);
void   ArraySetLength_BP(int len, void *a);
void   ArrayReleaseClasses_BP(void *a);
void  *ArrayPP_dtor(void *a);
void   ArrayReleaseClasses_PP(void *a);
void   Globals_resetHints();
void   BluePrint_ctor(void *bp, unsigned int index);
void   Standing_ctor(Standing *s);
void  *Standing_dtor(Standing *s);
void   Mission_ctor(void *m, int a, int b, int c);
int    Ship_getMaxShieldHP();
void   Status_rg_loadAgents(Status *self);
void   Status_rg_loadWanted(Status *self);
void   Status_rg_setCampaignMission(Status *self, void *m);
void   Status_rg_setShip(Status *self, int shipPtr);
void   Status_rg_setStation(Status *self, void *st);
}

// Status::resetGame()
//   Wipes the persistent player state back to a fresh new-game baseline: zeroes counters,
//   recreates the bonus/visibility arrays, blueprint list, agents/wanted, standing, the
//   starting mission, ship and station.
void Status::resetGame()
{
    char *self = (char *)this;
    int *settings = g_rg_settings;

    I(self, 0x8c)  = 0;
    char *srec = (char *)*settings;
    I(self, 0x1e4) = 0;
    I(self, 0x100) = 0;
    I(self, 0x104) = 0;
    I(self, 0x1b0) = 0;
    I(self, 0x1b4) = 0;
    I(self, 0x1b8) = 0;
    I(self, 0x1bc) = 0;
    I(self, 0x1c0) = 0;
    I(self, 0x1c4) = 0;
    I(self, 0x1c8) = 1;
    I(self, 0x1cc) = 0;
    I(self, 0x1d0) = 0;
    I(self, 0x10c) = 0;
    C(self, 0x110) = 0;
    I(self, 0x118) = 0;
    I(self, 0x11c) = 0;
    I(self, 0x124) = 0;
    I(self, 0x12c) = 0;
    // 0x1d4..0x1e0 set from a zero vector
    I(self, 0x1d4) = 0;
    I(self, 0x1d8) = 0;
    I(self, 0x1dc) = 0;
    I(self, 0x1e0) = 0;
    I(self, 0x13c) = 0;
    I(self, 0x134) = 0;
    I(self, 0x144) = 0;
    C(self, 0x120) = 0;
    C(self, 0x128) = 0;
    C(self, 0x130) = 0;
    C(self, 0x138) = 0;
    C(self, 0x140) = 0;
    C(self, 0x148) = 0;

    char hardcore = C(srec, 0x36);
    C(self, 0x111) = 0;
    I(self, 0x114) = hardcore != 0 ? 3 : 0;

    // recreate the per-game scratch station at 0x14c.
    if (P(self, 0x14c) != 0) {
        (((Station *)((Station *)P(self, 0x14c)))->dtor(), ::operator delete((void *)P(self, 0x14c)));
        I(self, 0x14c) = 0;
    }
    Station *scratch = (Station *)Status_opnew(0x34);
    ((Station *)(scratch))->ctor_default();
    P(self, 0x14c) = scratch;

    if (P(self, 0x28) != 0) {
        ::operator delete[](P(self, 0x28));
        I(self, 0x28) = 0;
    }
    if (P(self, 0x24) != 0) {
        ArrayReleaseClasses_String(P(self, 0x24));
        if (P(self, 0x24) != 0)
            ::operator delete(ArrayString_dtor(P(self, 0x24)));
    }

    // clear several bool arrays.
    int *ba = (int *)P(self, 0x94);
    I(self, 0x30) = 0;
    I(self, 0x24) = 0;
    for (int j = 0; *ba != j; j = j + 1)
        ((char *)ba[1])[j] = 0;

    ba = (int *)P(self, 0x98);
    for (int j = 0; *ba != j; j = j + 1)
        ((char *)ba[1])[j] = 0;

    ba = (int *)P(self, 0xac);
    I(self, 0x9c) = 0;
    I(self, 0xa0) = 0;
    I(self, 0xa4) = 0;
    I(self, 0xa8) = 0;
    for (int j = 0; *ba != j; j = j + 1)
        ((char *)ba[1])[j] = 0;

    ba = (int *)P(self, 0xb4);
    I(self, 0xb0) = 0;
    for (int j = 0; *ba != j; j = j + 1)
        ((char *)ba[1])[j] = 0;

    // free any stations on the stack at 0x1a0.
    unsigned *stk = (unsigned *)P(self, 0x1a0);
    if (stk != 0) {
        for (unsigned k = 0; k < *stk; k = k + 1) {
            Station *st = ((Station **)stk[1])[k];
            if (st != 0) {
                (((Station *)(st))->dtor(), ::operator delete((void *)st));
                ((int *)(*(int *)((char *)P(self, 0x1a0) + 4)))[k] = 0;
                stk = (unsigned *)P(self, 0x1a0);
            }
        }
    }

    Globals_resetHints();
    Galaxy *gal = *g_rg_galaxy;
    ((Galaxy *)(gal))->reset();

    ba = (int *)P(self, 0x50);
    for (int j = 0; *ba != j; j = j + 1)
        ((char *)ba[1])[j] = 0;

    // array at 0x54: first 0xb0 entries = 1, rest = 0.
    unsigned *arr54 = (unsigned *)P(self, 0x54);
    char *d54 = (char *)arr54[1];
    for (int j = 0; j != 0xb0; j = j + 1)
        d54[j] = 1;
    for (unsigned k = 0xb0; k < *arr54; k = k + 1)
        d54[k] = 0;

    d54[0x3e] = 0;
    *(short *)(d54 + 0x3c) = 0;
    I(self, 0x34) = 0;
    I(self, 0xb8) = 0;
    // zero the two 16-byte ship-equipment-slot vectors.
    I(self, 0xe0) = 0; I(self, 0xe4) = 0; I(self, 0xe8) = 0; I(self, 0xec) = 0;
    I(self, 0xc0) = 0; I(self, 0xc4) = 0; I(self, 0xc8) = 0; I(self, 0xcc) = 0;
    I(self, 0xd0) = 0; I(self, 0xd4) = 0; I(self, 0xd8) = 0; I(self, 0xdc) = 0;

    // a few PC-relative scalar globals get zeroed.
    *(*g_rg_zeroSlotA) = 0;
    **g_rg_zeroSlotB   = 0;
    *(*g_rg_zeroSlotC) = 0;

    I(self, 0x160) = 0;
    I(self, 0x164) = 0;
    C(self, 0x108) = 0;
    I(self, 0x174) = 0;
    *(short *)(self + 0xf0) = 0;
    I(self, 0xf4) = -1;
    *(short *)(self + 0xf8) = 1;

    ((Achievements *)(*g_rg_ach))->init();

    // recreate the four int arrays at 0x40/0x3c/0x48/0x44 (length 0xe9).
    static const int off4[4] = {0x40, 0x3c, 0x48, 0x44};
    for (int q = 0; q < 4; q = q + 1) {
        if (P(self, off4[q]) != 0) {
            ::operator delete(Array_int_dtor(P(self, off4[q])));
            I(self, off4[q]) = 0;
        }
    }
    for (int q = 0; q < 4; q = q + 1) {
        void *a = Status_opnew(0xc);
        Array_int_ctor(a);
        P(self, off4[q]) = a;
        ArraySetLength_int(0xe9, a);
    }
    {
        int *d40 = (int *)(*(int *)((char *)P(self, 0x40) + 4));
        int *d3c = (int *)(*(int *)((char *)P(self, 0x3c) + 4));
        int *d48 = (int *)(*(int *)((char *)P(self, 0x48) + 4));
        int *d44 = (int *)(*(int *)((char *)P(self, 0x44) + 4));
        for (int j = 0; j != 0xe9; j = j + 1) {
            d40[j] = 0; d3c[j] = 0; d48[j] = 0; d44[j] = 0;
        }
    }

    // bool array at 0x4c (length 4).
    if (P(self, 0x4c) != 0) {
        ::operator delete(Array_bool_dtor(P(self, 0x4c)));
        I(self, 0x4c) = 0;
    }
    {
        void *a = Status_opnew(0xc);
        Array_bool_ctor(a);
        P(self, 0x4c) = a;
        ArraySetLength_bool(4, a);
        char *d = (char *)(*(int *)((char *)P(self, 0x4c) + 4));
        for (int j = 0; j != 4; j = j + 1) d[j] = 0;
    }

    // bool array at 0x58 (length 5).
    if (P(self, 0x58) != 0) {
        ::operator delete(Array_bool_dtor(P(self, 0x58)));
        I(self, 0x58) = 0;
    }
    {
        void *a = Status_opnew(0xc);
        Array_bool_ctor(a);
        P(self, 0x58) = a;
        ArraySetLength_bool(5, a);
        char *d = (char *)(*(int *)((char *)P(self, 0x58) + 4));
        for (int j = 0; j != 5; j = j + 1) d[j] = 0;
    }

    // system visibilities at 0x38 from the galaxy systems.
    unsigned *systems = (unsigned *)((Galaxy *)(gal))->getSystems();
    void *vis = P(self, 0x38);
    for (unsigned k = 0; k < *systems; k = k + 1) {
        int v = ((SolarSystem *)(((void **)systems[1])[k]))->isVisible();
        ((char *)(*(int *)((char *)vis + 4)))[k] = (char)v;
    }

    // count craftable blueprints.
    unsigned bpCount = 0;
    int *itHolder = *g_rg_itemTable;
    unsigned *items = (unsigned *)*itHolder;
    for (unsigned k = 0; k < *items; k = k + 1) {
        if (((Item **)items[1])[k]->getIngredients() != 0)
            bpCount = bpCount + 1;
    }

    if (P(self, 0x18) != 0) {
        ArrayReleaseClasses_BP(P(self, 0x18));
        if (P(self, 0x18) != 0)
            ::operator delete(ArrayBP_dtor(P(self, 0x18)));
    }
    I(self, 0x18) = 0;
    if (bpCount != 0) {
        void *a = Status_opnew(0xc);
        ArrayBP_ctor(a);
        P(self, 0x18) = a;
        ArraySetLength_BP(bpCount, a);
        int idx = 0;
        for (unsigned k = 0; k < *items; k = k + 1) {
            if (((Item **)items[1])[k]->getIngredients() != 0) {
                void *bp = Status_opnew(0x2c);
                BluePrint_ctor(bp, k);
                ((void **)(*(int *)((char *)P(self, 0x18) + 4)))[idx] = bp;
                idx = idx + 1;
            }
        }
    }

    if (P(self, 0x1c) != 0) {
        ArrayReleaseClasses_PP(P(self, 0x1c));
        if (P(self, 0x1c) != 0)
            ::operator delete(ArrayPP_dtor(P(self, 0x1c)));
    }
    I(self, 0x1c) = 0;

    Status_rg_loadAgents(this);
    Status_rg_loadWanted(this);

    // recreate Standing at 0x14.
    if (P(self, 0x14) != 0) {
        ::operator delete(Standing_dtor((Standing *)P(self, 0x14)));
        I(self, 0x14) = 0;
    }
    Standing *st = (Standing *)Status_opnew(8);
    Standing_ctor(st);
    P(self, 0x14) = st;
    I(self, 0x7c) = -1;
    I(self, 0x80) = -1;
    I(self, 0x84) = 0;
    // missions[0] gets a base value from a status slot.
    *(int *)((*(int *)((char *)P(self, 0x198) + 4)) + 4) = **g_rg_statusSlotC;
    I(self, 0x1e8) = 0;

    void *mission = Status_opnew(0x78);
    Mission_ctor(mission, 4, 0, 0x4e);
    Status_rg_setCampaignMission(this, mission);

    int *slotB = *g_rg_statusSlotB;
    I(self, 0x194) = *(int *)(*(int *)((char *)P(self, 0x198) + 4));
    int newShip = (int)(intptr_t)((Ship *)(*(int *)((*(int *)(*slotB + 4)) + 0x28)))->makeShip(-1);  // price=-1 recovered via Ghidra
    Status_rg_setShip(this, newShip);
    ((Ship *)(*(Ship **)(self + 0x190)))->priceDecline();
    Status_rg_setStation(this, (void *)(intptr_t)((Galaxy *)(gal))->getStation(0));  // new-game home station (index 0)
    ((Ship *)(*(Ship **)(self + 0x190)))->setCargo(0);

    int (*makeItemB)(int) = g_rg_makeItemB;
    void (*addCargo)(int, int, int) = g_rg_addCargo;
    int shipObj = *(int *)(self + 0x190);
    int srcShip = *(int *)(*itHolder + 4);

    addCargo(shipObj, makeItemB(*(int *)(srcShip + 8)), 0);
    addCargo(shipObj, makeItemB(*(int *)(srcShip + 8)), 1);
    addCargo(shipObj, makeItemB(*(int *)(srcShip + 0xd8)), 0);
    addCargo(shipObj, makeItemB(*(int *)(srcShip + 0xec)), 1);
    addCargo(shipObj, makeItemB(*(int *)(srcShip + 0x148)), 2);
    addCargo(shipObj, makeItemB(*(int *)(srcShip + 0x124)), 3);
    addCargo(*(int *)((intptr_t)(*(int **)(*g_rg_statusSlotA)) + 0x190),
             (int)(intptr_t)((Item *)(*(int *)(srcShip + 0x90)))->makeItem(), 0);

    if (C(srec, 0x35) != 0)
        ((char *)(*(int *)((char *)P(self, 0x38) + 4)))[0x19] = 1;

    I(self, 0x64) = ((Ship *)(*(Ship **)(self + 0x190)))->getMaxHP();
    I(self, 0x5c) = Ship_getMaxShieldHP();
    I(self, 0x60) = ((Ship *)(*(Ship **)(self + 0x190)))->getMaxArmorHP();
    I(self, 0x68) = 100;
    I(self, 0x150) = -1;
    I(self, 0x154) = -1;
    I(self, 0x158) = -1;
}

struct SystemPathFinder {
    void *_opaque;
    int *getSystemPath(void *systems, int from, int to);
};

__attribute__((visibility("hidden"))) extern Status **g_anwStatus;     // bceec
__attribute__((visibility("hidden"))) extern int g_anwSysMask;          // bd290
__attribute__((visibility("hidden"))) extern int **g_anwRandStation;    // bd294
__attribute__((visibility("hidden"))) extern int *g_anwRandom;          // bd1dc

// Walks the wanted roster and activates any newly-eligible bounty, routing it onto a random
// reachable station pair. Returns the number of bounties activated.
int Status::activateNewWanted() {
    Status **slot = g_anwStatus;
    unsigned *w = (unsigned *)(*slot)->wanted;
    if (w == 0) {
        return 0;
    }
    void *systems = 0;
    int activated = 0;
    SystemPathFinder *pf = 0;
    for (unsigned i = 0; i < *w; i = i + 1) {
        Wanted *cur = ((Wanted **)w[1])[i];
        if (cur == 0) {
            continue;
        }
        unsigned lo, hi;
        if (i < 2) {
            hi = 4;
            lo = 2;
        } else {
            int rem = __aeabi_idivmod(i - 1, 6);
            lo = __aeabi_idiv(rem, 3) + 2;
            hi = rem / 2 + 4;
        }
        if (((SolarSystem *)((*slot)->system))->getRace() != ((Wanted *)(cur))->getBoard()) {
            continue;
        }
        if ((*slot)->currentCampaignMission < ((Wanted *)(cur))->getRequiredMission()) {
            continue;
        }
        if (i < 2 && (*slot)->currentCampaignMission > ((Wanted *)(cur))->getRequiredMission()) {
            continue;
        }
        if ((*slot)->inAlienOrbit() != 0) {
            continue;
        }
        if (Station_getIndex((*slot)->station) == 0x6c) {
            continue;
        }
        if (((Wanted *)(cur))->isActive() != 0) {
            continue;
        }
        if (((Wanted *)(cur))->isTerminated() != 0) {
            continue;
        }
        int board = ((Wanted *)(cur))->getBoard();
        int avail = (board < 4) ? (*slot)->collectedBounties[board] : 0;
        if (avail < ((Wanted *)(cur))->getRequiredBounties()) {
            continue;
        }
        ((Wanted *)(cur))->setActive(true);
        if (pf == 0) {
            pf = (SystemPathFinder *)operator new(1);
            SystemPathFinder_ctor(pf);
            FileRead *fr = (FileRead *)operator new(1);
            FileRead_ctor(fr);
            systems = (void *)fr->loadSystemsBinary();
            ::operator delete(FileRead_dtor(fr));
        }
        activated = activated + 1;
        int *path;
        int fromSys, toSys;
        do {
            Station *a = Globals_getRandomStation();
            unsigned asys = Station_getSystem(a);
            unsigned *vis = (unsigned *)(*slot)->systemVisibilities;
            bool ok;
            while (true) {
                ok = false;
                if (vis != 0 && asys < *vis) {
                    ok = *(char *)(vis[1] + asys) != 0;
                }
                if (((SolarSystem *)((SolarSystem *)(void *)asys))->getRoutes() != 0 && ok &&
                    !(asys < 0x1d && (1 << (asys & 0xff) & g_anwSysMask) != 0)) {
                    break;
                }
                if (a != 0) {
                    (((Station *)(a))->dtor(), ::operator delete((void *)a));
                }
                a = Globals_getRandomStation();
                asys = Station_getSystem(a);
            }
            Station *b = Globals_getRandomStation();
            unsigned bsys = Station_getSystem(b);
            bool ok2;
            while (true) {
                ok2 = false;
                if (vis != 0 && bsys < *vis) {
                    ok2 = *(char *)(vis[1] + bsys) != 0;
                }
                if (((SolarSystem *)((SolarSystem *)(void *)bsys))->getRoutes() != 0 && ok2 &&
                    !(bsys < 0x1d && (1 << (bsys & 0xff) & g_anwSysMask) != 0) && asys != bsys) {
                    break;
                }
                if (b != 0) {
                    (((Station *)(b))->dtor(), ::operator delete((void *)b));
                }
                b = Globals_getRandomStation();
                bsys = Station_getSystem(b);
            }
            ((Wanted *)(cur))->setLastSeen(Station_getIndex(a));
            ((Wanted *)(cur))->setTravelsTo(Station_getIndex(b));
            fromSys = Station_getSystem(a);
            toSys = Station_getSystem(b);
            if (a != 0) {
                (((Station *)(a))->dtor(), ::operator delete((void *)a));
            }
            if (b != 0) {
                (((Station *)(b))->dtor(), ::operator delete((void *)b));
            }
            path = ((SystemPathFinder *)(pf))->getSystemPath(systems, fromSys, toSys);
        } while (path == 0 || (unsigned)*path < lo || hi < (unsigned)*path);
        int *rnd = g_anwRandom;
        int pick = path[1] + ((AbyssEngine::AERandom *)(*rnd))->nextInt() * 4;
        pick = *(int *)pick;
        SolarSystem *dst = (SolarSystem *)((int *)((unsigned *)systems)[1])[pick];
        if (((SolarSystem *)(dst))->getStations_i() != 0) {
            int idx = ((AbyssEngine::AERandom *)(*rnd))->nextInt();
            int st = ((int *)(*(int *)(((SolarSystem *)(dst))->getStations_i() + 4)))[idx];
            ((Wanted *)(cur))->setCurrentLocation(st);
        }
        ::operator delete(Array_int_dtor(path));
    }
    if (pf != 0) {
        ::operator delete(SystemPathFinder_dtor(pf));
    }
    if (systems != 0) {
        ArrayReleaseClasses_SolarSystem(systems);
        ::operator delete(Array_SolarSystem_dtor(systems));
    }
    return activated;
}

struct PendingProduct;

// Merges `bp` into the pending-products list, bumping an existing matching entry's quantity
// or appending a freshly constructed PendingProduct.
void Status::addPendingProduct(BluePrint *bp) {
    unsigned *arr = (unsigned *)pendingProducts;
    if (arr == 0) {
        int *na = (int *)operator new(0xc);
        Array_PendingProduct_ctor(na);
        pendingProducts = na;
    } else {
        for (unsigned i = 0; i < *arr; i = i + 1) {
            int e = ((int *)arr[1])[i];
            if (e != 0 && *(int *)(e + 0x14) == ((BluePrint *)(bp))->getIndex() &&
                *(int *)(((int *)((int *)pendingProducts)[1])[i] + 0xc) ==
                    ((BluePrint *)(bp))->getStationIndex()) {
                int q = ((BluePrint *)(bp))->getQuantity();
                int slot = ((int *)((int *)pendingProducts)[1])[i];
                *(int *)(slot + 0x10) = q + *(int *)(slot + 0x10);
                return;
            }
            arr = (unsigned *)pendingProducts;
        }
    }
    int *pp = (int *)operator new(0x18);
    PendingProduct_ctor(pp, bp);
    ((PendingProduct *)(pp))->add(*(Array<PendingProduct*> *)pendingProducts);
}

__attribute__((visibility("hidden"))) extern int g_emptyOrbitMask;

// True when the current orbit has no station/special content for the player to interact with.
bool Status::inEmptyOrbit() {
    int idx = Station_getIndex(station);
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
    FileRead *fr = (FileRead *)operator new(1);
    FileRead_ctor(fr);
    wanted = fr->loadWanted();
    FileRead_dtor(fr);
    operator_delete_tail(fr);
}

// Unlocks every blueprint whose item index matches `index`.
void Status::unlockBluePrint(int index) {
    Array<BluePrint *> *bps = bluePrints;
    for (unsigned i = 0; i < bps->size(); i = i + 1) {
        BluePrint *bp = (*bps)[i];
        if (((BluePrint *)(bp))->getIndex() == index) {
            ((BluePrint *)((*bluePrints)[i]))->unlock();
        }
    }
}

// Per-second gamma-ray damage near supernova/storm orbits, keyed by station/system ids.
// Returns the float result as raw bits (the engine treats the value as float at the call site).
__attribute__((visibility("hidden"))) extern const float g_gammaTableA[5];
__attribute__((visibility("hidden"))) extern const float g_gammaTableB[5];

static inline int as_int(float f) { union { float f; int i; } u; u.f = f; return u.i; }
static inline float as_float(unsigned u) { union { unsigned u; float f; } x; x.u = u; return x.f; }

int Status::getGammaRayDamagePerSecond(int a, int b) {
    unsigned k = a - 0x6d;
    float result = as_float(0x68409917u);
    if (k < 5) {
        if (b < 0x6a) {
            if (k < 5) return as_int(g_gammaTableA[k]);
        } else if (currentCampaignMission < 0x9e) {
            if (k < 5) return as_int(g_gammaTableB[k]);
        } else if (a == 0x6d) {
            result = as_float(0x3f800000u);
        }
    }
    return as_int(result);
}

// Pushes `s` onto the 3-deep station stack (shifting older entries down), unless it is
// already present. Returns 1 if the stack was modified, 0 otherwise.
int Status::addStationToStack(Station *s) {
    Station *found = (Station *)(intptr_t)((Status *)(this))->isOnStack(s);
    if (found != 0) {
        ((Status *)(this))->setStationTail(found);
        return 0;
    }
    Station **base = stationStack->data();
    if (base[0] == 0) {
        int i = 0;
        int j;
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
            (((Station *)(base[2]))->dtor(), ::operator delete((void *)base[2]));
        }
        base[2] = 0;
        for (int i = 0; i != -2; i = i - 1) {
            base[i + 2] = base[i + 1];
        }
        base[0] = s;
    }
    ((Status *)(this))->setStationTail(s);
    return 1;
}

__attribute__((visibility("hidden"))) extern int *g_missionSentinel;

// Removes `mission` from the active list: clears the campaign slot, detaches its agent, and
// resets the slot to the sentinel value.
void Status::removeMission(Mission *mission) {
    int *sentinel = g_missionSentinel;
    Mission **slots = missions->data();
    for (unsigned i = 0; i < missions->size(); i = i + 1) {
        if (slots[i] == mission) {
            Mission *target = mission;
            if (this->mission == mission) {
                mission = 0;
                this->mission = 0;
                target = slots[i];
            }
            if (((Mission *)(target))->getAgent() != 0) {
                Agent *ag = (Agent *)((Mission *)(slots[i]))->getAgent();
                ((Agent *)(ag))->setMission(0);
            }
            int *slot = (int *)slots[i];
            if (slot == 0) {
                slots[i] = 0;
            } else {
                (*(void (**)(int *))(*slot + 4))(slot);
                slots[i] = 0;
            }
            slots[i] = (Mission *)(intptr_t)*sentinel;
        }
    }
}

struct MissionObj;
__attribute__((visibility("hidden"))) extern Status **g_ncmStatus;
__attribute__((visibility("hidden"))) extern int g_ncmAchTable[3];   // DAT_000b9d14 (3 mission ids)

extern "C" {
// Mission::Mission(int,int,int)
// Mission::Mission()
void Status_setCampaignMission_ncm(Status *self, void *m);           // setCampaignMission
void Status_addMissionTail(Status *self, void *m);                   // indirect tail 0x1abf38
void Ship_setCargo_ncm(Ship *ship, void *arr);                       // Ship::setCargo
void Ship_removeAllCargo_ncm(Ship *ship);                            // Ship::removeAllCargo
void *Ship_getEquipment_ncm(Ship *ship);                            // Ship::getEquipment
void *Ship_getCargo_ncm();                                          // Ship::getCargo
void Item_setUnsaleable_ncm(void *item, int v);
int  Item_getSinglePrice_ncm(void *item);
void Item_setPrice_ncm(void *item, int price);
void *Ship_getFirstEquipmentOfSort_ncm(int ship);
void Ship_removeEquipment_ncm(Ship *ship, void *item);
void Ship_addEquipment_ncm(Ship *ship, void *item);
void *Item_makeItem_ncm(int infoPtr);
}

// Per-campaign-step mission descriptor (the bulk of the switch is "build Mission(type,param,
// station) and register it" -- either as the campaign mission or via the add-mission tail).
struct Step { short type; int param; short station; unsigned char campaign; };

// Table indexed by campaign step (recovered from the engine's switch). Steps with extra side
// effects are handled specially below; the rest follow this descriptor.
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

// Status::nextCampaignMission(bool param_1)
//   Advances the campaign by one step: bumps the campaign-mission counter, snapshots the
//   playing-time, runs the achievement check, then spawns and registers the next scripted
//   Mission for that step (with a few steps performing extra cargo/equipment bookkeeping).
void Status::nextCampaignMission() {
    Status *self = this;
    int *state = (int *)((char *)self + 0x1e8);
    int prevTimeLo = (int)self->playingTime;
    int prevTimeHi = (int)(self->playingTime >> 32);

    int step = *state;
    int next = step + 1;

    // achievement check: flag if the new step matches any of the three tracked mission ids.
    for (int k = 0; k < 3; k = k + 1) {
        if (next == g_ncmAchTable[k])
            self->field_0x17c = 1;
    }

    // advance counter + snapshot time (cases that finalise via campaign use 0x1e8, others 0x1e8
    // too -- both write the same counter in the engine).
    *state = next;
    self->field_0x100 = prevTimeLo;
    self->field_0x104 = prevTimeHi;

    if (step == 0)
        return;   // step 0 has no mission.

    // --- steps with extra side effects ------------------------------------
    if (step == 3) {
        Ship_setCargo_ncm(self->ship, 0);
    } else if (step == 5) {
        Ship_removeAllCargo_ncm(self->ship);
    } else if (step == 7) {
        unsigned *eq = (unsigned *)Ship_getEquipment_ncm(self->ship);
        if (eq != 0)
            for (unsigned i = 0; i < *eq; i = i + 1) {
                void *it = *(void **)(eq[1] + i * 4);
                if (it != 0) { Item_setUnsaleable_ncm(it, 0); Item_setPrice_ncm(it, Item_getSinglePrice_ncm(it)); }
            }
        unsigned *cg = (unsigned *)Ship_getCargo_ncm();
        if (cg != 0)
            for (unsigned i = 0; i < *cg; i = i + 1) {
                void *it = *(void **)(cg[1] + i * 4);
                if (it != 0) { Item_setUnsaleable_ncm(it, 0); Item_setPrice_ncm(it, Item_getSinglePrice_ncm(it)); }
            }
    } else if (step == 9) {
        void *old = Ship_getFirstEquipmentOfSort_ncm((int)(long)self->ship);
        Ship_removeEquipment_ncm(self->ship, old);
        Ship_addEquipment_ncm(self->ship, Item_makeItem_ncm(0x158));
    }

    // --- spawn + register the step's mission -------------------------------
    void *m = ::operator new(0x78);
    if ((unsigned)step < sizeof(kSteps) / sizeof(kSteps[0])) {
        const Step &s = kSteps[step];
        if (s.type == 0 && s.station == 0 && s.param == 0) {
            ((Mission *)(m))->ctor_default();
        } else {
            ((Mission *)(m))->ctor3(s.type, s.param, s.station);
        }
        if (s.campaign)
            Status_setCampaignMission_ncm(self, m);
        else
            Status_addMissionTail(self, m);
    } else {
        // beyond the scripted range: default freelance hop.
        ((Mission *)(m))->ctor3(0xb, 0, 100);
        Status_addMissionTail(self, m);
    }
}

struct Galaxy;
struct SolarSystem;

__attribute__((visibility("hidden"))) extern Galaxy **g_galaxy;

// Tail dispatch shared by addStationToStack()/visitStation(): commits `s` as the current
// station by routing through the full setStation() travel logic.
void Status::setStationTail(Station *s) {
    setStation(s);
}

// Travels to `s`: marks its system visible, then rebuilds the planet name and texture tables
// for every station in the destination system.
void Status::setStation(Station *s) {
    if (station == s) {
        return;
    }
    station = s;
    Galaxy *gal = *g_galaxy;
    system = ((Galaxy *)(gal))->getSystem(Station_getSystem(s));
    if (system == 0) {
        return;
    }
    (*field_b4)[Station_getSystem(station)] = true;
    FileRead *fr = (FileRead *)operator new(1);
    FileRead_ctor(fr);
    Array<Station *> *list = (Array<Station *> *)((FileRead *)(fr))->loadStationsBinary();
    ::operator delete(FileRead_dtor(fr));
    if (planetNames != 0) {
        ArrayReleaseClasses_String((Array<String *> *)(void *)(uint32_t)planetNames);
        if (planetNames != 0) {
            ::operator delete(Array_String_dtor((Array<String *> *)(void *)(uint32_t)planetNames));
        }
    }
    planetNames = 0;
    Array<String *> *names = (Array<String *> *)operator new(0xc);
    Array_String_ctor(names);
    planetNames = (int32_t)(intptr_t)names;
    ArraySetLength_String(list->size(), names);
    if (planetTextures != 0) {
        ArrayRelease_int((void *)(uint32_t)planetTextures);
        if (planetTextures != 0) {
            ::operator delete(Array_int_dtor((void *)(uint32_t)planetTextures));
        }
    }
    planetTextures = 0;
    void *texs = operator new(0xc);
    Array_int_ctor(texs);
    planetTextures = (int32_t)(intptr_t)texs;
    ArraySetLength_int(list->size(), texs);
    Array<Station *> *stations = (Array<Station *> *)((SolarSystem *)((SolarSystem *)(void *)(uint32_t)system))->getStations();
    unsigned i = 0;
    while (true) {
        if (((Array<Station *> *)((SolarSystem *)((SolarSystem *)(void *)(uint32_t)system))->getStations())->size() <= i) {
            break;
        }
        Station *cur;
        unsigned j = 0;
        while (true) {
            if (list->size() <= j) {
                goto next;
            }
            cur = (*list)[j];
            int target = (int)(intptr_t)(*stations)[i];
            int idx = Station_getIndex(cur);
            j = j + 1;
            if (target == idx) {
                break;
            }
        }
        {
            int ccm = currentCampaignMission;
            String *nm = (String *)operator new(0xc);
            if (ccm == 0) {
                String_ctor_empty(nm);
            } else {
                ((Station *)(nm))->getName();
            }
            (*((Array<String *> *)(void *)(uint32_t)planetNames))[i] = nm;
            (*((Array<int> *)(void *)(uint32_t)planetTextures))[i] = Station_getTextureIndex(cur);
        }
    next:
        i = i + 1;
    }
    ArrayReleaseClasses_Station(list);
    Array_Station_dtor_tail(list);
}

// 12-byte AbyssEngine::String, built/destroyed via engine wrappers. Modeled locally as
// Str12 (text*, size, ...) — distinct from AbyssEngine::String12 (char16_t buf[6]) which
// the included headers pull into scope; the recovered code accesses .a/.b/.c by offset.
struct Str12 { uint32_t a, b, c; };

extern "C" {
int  String12_IndexOf(Str12 *self, Str12 *needle);
void String12_SubString(Str12 *out, Str12 *self, int start, int len);
void String12_ctor_copy(Str12 *self, const Str12 *src, bool copy);
void String12_concat(Str12 *out, Str12 *left, Str12 *right);     // 0x6ef98 operator+
void String12_dtor(Str12 *self);                                       // 0x6ee30 ~String
void String12_dtor_v(Str12 *self);                                     // dtor via DAT (blx r4)
}

// Status::replaceHash(String haystack, String needle, String replacement) -> String (sret)
//   Replaces the first occurrence of `needle` in `haystack` with `replacement`. If `needle`
//   is absent, returns a copy of `haystack`. `out` is the hidden return slot.
void Status_replaceHash3(void *self, Str12 *out, Str12 *haystack,
                                    Str12 *needle, Str12 *replacement)
{
    (void)self;
    int idx = String12_IndexOf(haystack, needle);
    if (idx < 0) {
        // no match -> copy haystack into the result.
        String12_ctor_copy(out, haystack, false);
        return;
    }

    // prefix = haystack[0 .. idx)
    Str12 prefix;
    String12_SubString(&prefix, haystack, 0, idx);

    if (prefix.b == 0) {
        // needle at position 0: result = replacement + haystack[idx+len ..)
        Str12 repl;
        String12_ctor_copy(&repl, replacement, false);
        Str12 suffix;
        String12_SubString(&suffix, haystack, needle->c + idx, haystack->c);
        String12_concat(out, &repl, &suffix);
        String12_dtor_v(&suffix);
        String12_dtor_v(&repl);
    } else {
        // result = prefix + replacement + haystack[idx+len ..)
        Str12 repl;
        String12_ctor_copy(&repl, replacement, false);
        Str12 mid;
        String12_concat(&mid, &prefix, &repl);
        Str12 suffix;
        String12_SubString(&suffix, haystack, needle->c + idx, haystack->c);
        String12_concat(out, &mid, &suffix);
        String12_dtor_v(&suffix);
        String12_dtor_v(&mid);
        String12_dtor_v(&repl);
    }
    String12_dtor(&prefix);
}

// 12-byte AbyssEngine::String, built/destroyed via engine wrappers (Str12 defined above).

extern "C" {
void String12_ctor_char(Str12 *self, const char *text, bool copy);
}

// Status::replaceHash(AbyssEngine::String haystack, AbyssEngine::String needle) -> String (sret)
//   Forwards to the three-argument overload, substituting the hash placeholder with "".
//   `out` is the hidden return slot; haystack/needle are passed by value (as Str12*).
void Status_replaceHash2(void *self, Str12 *out, Str12 *haystack, Str12 *needle)
{
    Str12 h, n, empty;
    String12_ctor_copy(&h, haystack, false);
    String12_ctor_copy(&n, needle, false);
    String12_ctor_char(&empty, "", false);
    Status_replaceHash3(self, out, &h, &n, &empty);
    String12_dtor(&empty);
    String12_dtor(&n);
    String12_dtor(&h);
}

// Adjusts the player rating by `delta`, clamping the result to [-10, 10].
void Status::changeRating(int delta) {
    int updated = delta + rating;
    rating = updated;
    int clamped;
    if (updated < 0xb) {
        if (-0xb < updated) {
            return;
        }
        clamped = -10;
    } else {
        clamped = 10;
    }
    rating = clamped;
}

struct SolarSystem;
struct Galaxy;

__attribute__((visibility("hidden"))) extern Status **g_dsStatus;     // DAT_000b962c/9630
__attribute__((visibility("hidden"))) extern int g_dsTypeMask;
__attribute__((visibility("hidden"))) extern int *g_dsRandom;
__attribute__((visibility("hidden"))) extern Galaxy **g_dsGalaxy;

extern "C" {
int Status_inAlienOrbit_ds();
int Station_getIndex(Station *s);
void *Station_getItems_ds();
int Station_getShips_ds();
int isOnStack_ds(Status *self, Station *s);
void addStationToStack_ds(Status *self, Station *s);
void Generator_ctor(Generator *g);
void *Generator_getItemBuyList(Generator *g, Station *s);
int Generator_getShipBuyList(Station *g);
void *Generator_createAgents(Generator *g, Station *s);
int Mission_getType_ds();
int Mission_isCampaignMission_ds(Mission *m);
int Mission_getProductionGoodIndex_ds(Mission *m);
int Mission_isEmpty_ds();
int Mission_getTargetStation_ds();
int Ship_hasCargo_ds(int ship, int good);
int AERandom_nextInt_ds(int rng, int bound);
void *Galaxy_getSystem_ds(Galaxy *g, int sysIdx);
int SolarSystem_getStations_ds(void *sys);
void *Ship_getCargo_ds();
void Item_setStationAmount_ds(void *item, int v);
void Status_departTail(int ship);                                    // tail 0x1abf08
}

// Byte-offset helpers for fields that live in padding regions of the recovered Status struct.
static inline int &SF(Status *s, unsigned off) { return *(int *)((char *)s + off); }

// Status::departStation(Station* dest)   [self in r0, dest in r1]
//   Leaving the current station to travel to `dest`: refreshes inventories, stacks the prior
//   station, regenerates the destination's wares/ships/agents, then re-targets the active
//   campaign/freelance mission, moves wanted criminals, and clears per-cargo station amounts.
void Status::departStation(Station *dest) {
    Status *self = this;

    // refresh the just-departed station inventory if it is the campaign hub in state 3.
    if (Status_inAlienOrbit_ds() == 0) {
        bool hub = Station_getIndex(self->station) == 0x6c;
        if (hub && SF(self, 0x114) == 3) {
            Station *st = (Station *)(void *)(long)SF(self, 0x14c);
            ((Station *)(st))->setItems((uint32_t *)Station_getItems_ds(), true);
            ((Station *)((void *)(long)SF(self, 0x14c)))->setShips((uint32_t *)(long)Station_getShips_ds(), true);
        }
    }

    bool wantedMove = false;
    if (Status_inAlienOrbit_ds() == 0 && self->playerStation != dest) {
        wantedMove = Station_getIndex(dest) != Station_getIndex(self->station);
    }

    if (self->playerStation != dest) {
        Station *prev = (Station *)(void *)(long)isOnStack_ds(self, dest);
        addStationToStack_ds(self, dest);
        bool hub = Station_getIndex(dest) == 0x6c;
        if (hub && SF(self, 0x114) == 3) {
            ((Station *)(dest))->setItems((uint32_t *)Station_getItems_ds(), true);
            ((Station *)((void *)dest))->setShips((uint32_t *)(long)Station_getShips_ds(), true);
            if (prev != 0 && prev != dest) {
                ((Station *)(prev))->setItems((uint32_t *)Station_getItems_ds(), true);
                ((Station *)((void *)prev))->setShips((uint32_t *)(long)Station_getShips_ds(), true);
            }
            if (prev == 0) {
                Generator g;
                Generator_ctor(&g);
                ((Station *)(dest))->setAgents(Generator_createAgents(&g, dest));
            }
        } else if (prev == 0) {
            Generator g;
            Generator_ctor(&g);
            ((Station *)(dest))->setItems((uint32_t *)Generator_getItemBuyList(&g, dest), false);
            ((Station *)((void *)dest))->setShips((uint32_t *)(long)Generator_getShipBuyList((Station *)&g), false);
            ((Station *)(dest))->setAgents(Generator_createAgents(&g, dest));
        }
        SF(self, 0x70) = (int)self->playingTime;
        SF(self, 0x74) = (int)(self->playingTime >> 32);
    }

    // pick the active mission target for the new station.
    self->mission = (Mission *)*(void **)((char *)(*g_dsStatus));   // reset to default
    Status *st = *g_dsStatus;
    Array<Mission *> *missions = self->missions;
    for (unsigned i = 0; i < *(unsigned *)missions; i = i + 1) {
        Mission *mi = ((Mission **)(*(char **)((char *)missions + 4)))[i];
        if (mi == 0)
            continue;
        int type = Mission_getType_ds();
        int camp = Mission_isCampaignMission_ds(mi);

        if (type == 0xad && camp != 0) {
            int good = Mission_getProductionGoodIndex_ds(mi);
            if (Ship_hasCargo_ds((int)(long)st->ship, good) != 0) {
                self->mission = mi;
                break;
            }
        }
        if (st->currentCampaignMission < 0x2d && type == 0xa1 && camp != 0 &&
            Station_getIndex(dest) == SF(self, 0x80) && Status_inAlienOrbit_ds() == 0) {
            self->mission = mi;
            break;
        }
        if (Mission_isEmpty_ds() == 0) {
            int tgt = Mission_getTargetStation_ds();
            if (tgt == Station_getIndex(dest) &&
                ((unsigned)(type - 0x96) > 0x17 || ((1 << ((type - 0x96) & 0xff)) & g_dsTypeMask) == 0) &&
                type != 8 && type != 0xe) {
                if (camp != 0 || (type != 0xb && type != 0xd)) { self->mission = mi; break; }
            }
        }
        if (Mission_isEmpty_ds() == 0 && camp != 0) {
            if (type == 0xa0 && Mission_getTargetStation_ds() != Station_getIndex(dest)) {
                self->mission = mi;
                break;
            }
        }
    }

    // recompute the next freelance destination station.
    if (self->currentCampaignMission < 0x2d) {
        if (self->currentCampaignMission >= 0x20) {
            bool same = Station_getIndex(dest) == Mission_getTargetStation_ds() ||
                        Station_getIndex(dest) == SF(self, 0x80);
            if (!same) {
                if (SF(self, 0x8c) + 1 >= 10) {
                    SF(self, 0x8c) = 0;
                    int sys;
                    do {
                        do {
                            sys = AERandom_nextInt_ds(*g_dsRandom, 0x16);
                            SF(self, 0x7c) = sys;
                        } while ((*self->systemVisibilities)[sys] == 0);
                    } while (sys == 10 || sys == 0xf);
                    void *ss = Galaxy_getSystem_ds(*g_dsGalaxy, SF(self, 0x7c));
                    int stations = SolarSystem_getStations_ds(ss);
                    int n = AERandom_nextInt_ds(*g_dsRandom, SolarSystem_getStations_ds(ss));
                    SF(self, 0x80) = *(int *)(*(int *)(stations + 4) + n * 4);
                } else {
                    SF(self, 0x8c) = SF(self, 0x8c) + 1;
                }
            }
        }
    } else {
        SF(self, 0x7c) = -10;
        SF(self, 0x80) = -10;
    }

    if (wantedMove)
        self->moveWanted();

    // clear the per-cargo station-amount counters.
    unsigned *cargo = (unsigned *)Ship_getCargo_ds();
    if (cargo != 0) {
        for (unsigned i = 0; i < *cargo; i = i + 1) {
            void *it = *(void **)(cargo[1] + i * 4);
            if (it != 0)
                Item_setStationAmount_ds(it, 0);
        }
    }
    self->field_0x108 = 0;
    Status_departTail((int)(long)self->ship);
}

// Singleton Status holders referenced via PC-relative slots.
__attribute__((visibility("hidden"))) extern Status **g_mcStatusA;   // DAT_000bc4c4 etc.
__attribute__((visibility("hidden"))) extern Status **g_mcStatusB;

extern "C" {
int Station_getIndex(Station *s);
int Item_isInList2(int a, void *list);
void *BluePrint_getIngredientList(void *bp);
void *BluePrint_getQuantityList(void *bp);
int Status_inAlienOrbit3();
}

// Status::missionCompleted(bool atStation, bool docked, long long extra)
//   Scans the active mission list; for the first unfinished mission whose completion criteria
//   are met it returns that Mission (some types auto-mark themselves won). Mission-type-specific
//   checks mirror the engine's giant dispatch.
Mission * Status::missionCompleted(bool atStation, bool docked, long long extra) {
    Status *self = this;
    unsigned which = (unsigned)docked;
    unsigned both = which & (unsigned)extra;

    Array<Mission *> *missions = self->missions;
    for (unsigned i = 0; i < *(unsigned *)missions; i = i + 1) {
        Mission *m = ((Mission **)(*(char **)((char *)missions + 4)))[i];
        if (((Mission *)(m))->hasWon() != 0)
            return (Mission *)0;

        // skip irrelevant missions (no agent/target/campaign tie-in).
        if (m == 0 ||
            (((Mission *)(m))->isCampaignMission() == 0 && ((Mission *)(m))->getAgent() == 0 &&
             ((Mission *)(m))->getClientImage() == 0 && ((Mission *)(m))->getTargetStation() == 0 &&
             ((Mission *)(m))->getTargetStation() == 0))
            continue;

        int type = ((Mission *)(m))->getType();
        switch (type) {
        case 0x96:
            if (((Mission *)(m))->getStatusValue() <= self->missionCount) { ((Mission *)(m))->setWon(true); return m; }
            break;
        case 0x97:
            if (((Mission *)(m))->getStatusValue() <= self->kills) { ((Mission *)(m))->setWon(true); return m; }
            break;
        case 0x99:
            if (((Mission *)(m))->getStatusValue() <= self->stationsVisited) { ((Mission *)(m))->setWon(true); return m; }
            break;
        case 0x9b:
            if (((Mission *)(m))->getStatusValue() <= self->goodsProduced) { ((Mission *)(m))->setWon(true); return m; }
            break;
        case 0x98: {
            unsigned *eq = (unsigned *)((Ship*)(self->ship))->getEquipment();
            for (unsigned j = 0; j < *eq; j = j + 1) {
                void *it = *(void **)(eq[1] + j * 4);
                if (it != 0 && ((Item *)(it))->getIndex() == ((Mission *)(m))->getStatusValue())
                    return m;
            }
            break;
        }
        case 0x9a: {
            if (((Mission *)(m))->getProductionGoodAmount() <= ((Ship *)(self->ship))->getCurrentLoad()) { ((Mission *)(m))->setWon(true); return m; }
            break;
        }
        case 0xa7:
        case 0xae:
            if (((Mission *)(m))->getProductionGoodAmount() <= ((Mission *)(m))->getStatusValue()) { ((Mission *)(m))->setWon(true); return m; }
            break;
        case 0x9c:
            if (!docked) {
                int here = Station_getIndex(self->station);
                int target = ((Mission *)(m))->getTargetStation();
                if (extra >= 0x2711 && here == target) return m;
            }
            break;
        case 0xa0:
            if (docked) return m;
            {
                int here = Station_getIndex(self->station);
                int target = ((Mission *)(m))->getTargetStation();
                if (extra < 0x2711 && here != target) return m;
            }
            break;
        case 0x9d: {
            unsigned *eq = (unsigned *)((Ship*)(self->ship))->getEquipment();
            for (unsigned j = 0; j < *eq; j = j + 1) {
                void *it = *(void **)(eq[1] + j * 4);
                if (it != 0 && ((Item *)(it))->getType() == ((Mission *)(m))->getStatusValue())
                    return m;
            }
            break;
        }
        case 0x9e: {
            unsigned *eq = (unsigned *)((Ship*)(self->ship))->getEquipment();
            bool hasGood = false, hasSpecial = false;
            for (unsigned j = 0; j < *eq; j = j + 1) {
                void *it = *(void **)(eq[1] + j * 4);
                if (it != 0) {
                    if (((Item *)(it))->getType() == 0)
                        hasGood = true;
                    else if (((Item *)(it))->getSort() == 10)
                        hasSpecial = true;
                }
            }
            if (hasGood && hasSpecial) return m;
            break;
        }
        case 0xa2:
            if (docked) {
                if (Station_getIndex(self->station) == ((Mission *)(m))->getTargetStation()) {
                    void *bp = *(void **)((char *)self->bluePrints + ((Mission *)(m))->getStatusValue() * 4);
                    unsigned *ing = (unsigned *)BluePrint_getIngredientList(bp);
                    unsigned *qty = (unsigned *)BluePrint_getQuantityList(bp);  // amount list, parallel to ingredients (Ghidra)
                    bool all = true;
                    for (unsigned j = 0; j < *ing; j = j + 1) {
                        if (((Ship *)((int)(long)self->ship))->hasCargo(*(int *)(ing[1] + j * 4), *(int *)(qty[1] + j * 4)) == 0) { all = false; break; }
                    }
                    if (all) return m;
                }
            }
            break;
        case 0xa3: {
            unsigned *arr = (unsigned *)self->field_90;
            bool all = true;
            for (unsigned j = 0; j < *arr; j = j + 1) {
                if (*(int *)(arr[1] + j * 4) < 0) { all = false; break; }
            }
            if (all) return m;
            break;
        }
        case 0xa8: {
            Status *s = *g_mcStatusA;
            if (((Mission *)(m))->getStatusValue() <= s->field_178) {
                ((Mission *)(m))->setWon(true);
                s->field_178 = 0;
                return m;
            }
            break;
        }
        case 0xaa:
            if (((Mission *)(m))->getStatusValue() == 1) { ((Mission *)(m))->setWon(true); return m; }
            break;
        case 0xab:
            if (docked) {
                if (Station_getIndex(self->station) == ((Mission *)(m))->getTargetStation()) return m;
            }
            break;
        case 0xac:
            if (both != 0) {
                if (Station_getIndex(self->station) == ((Mission *)(m))->getTargetStation()) {
                    int idx = ((Mission *)(m))->getProductionGoodIndex();
                    int amt = ((Mission *)(m))->getProductionGoodAmount();
                    if (Item::isInList(idx, amt, (ItemArray *)((Ship *)(self->ship))->getCargo()) != 0) return m;
                }
            }
            break;
        case 0xa6:
            if (docked) {
                if (Station_getIndex(self->station) == ((Mission *)(m))->getTargetStation()) {
                    int idx = ((Mission *)(m))->getProductionGoodIndex();
                    int amt = ((Mission *)(m))->getProductionGoodAmount();
                    if (Item::isInList(idx, amt, (ItemArray *)((Ship *)(self->ship))->getCargo()) != 0) return m;
                    if (((Ship *)((int)(long)((*g_mcStatusA)->ship)))->hasEquipment(((Mission *)(m))->getProductionGoodIndex(), 1) == 0) return m;  // amount=1 recovered via Ghidra
                }
            }
            break;
        case 0xb8:
            if (((Mission *)(m))->getStatusValue() == 0) {
                Status *s = *g_mcStatusB;
                if (s->currentCampaignMission != 0x5c || Status_inAlienOrbit3() != 0 ||
                    Station_getIndex(s->station) != 0x71) {
                    ((Mission *)(m))->setWon(true);
                    return m;
                }
            }
            break;
        case 0xbd:
            if (docked) {
                unsigned *eq = (unsigned *)((Ship*)(self->ship))->getEquipment();
                for (unsigned j = 0; j < *eq; j = j + 1) {
                    void *it = *(void **)(eq[1] + j * 4);
                    if (it != 0 && ((Item *)(it))->getSort() == ((Mission *)(m))->getStatusValue())
                        return m;
                }
            }
            break;
        default:
            // legacy mission ids 8..0xe.
            if (type == 8) {
                if (docked && Station_getIndex(self->station) == ((Mission *)(m))->getTargetStation()) {
                    int idx = ((Mission *)(m))->getProductionGoodIndex();
                    int amt = ((Mission *)(m))->getProductionGoodAmount();
                    if (Item::isInList(idx, amt, (ItemArray *)((Ship *)(self->ship))->getCargo()) != 0) return m;
                }
            } else if (type == 0xd) {
                if (docked && self->field_0xf0 != 0) return m;
            } else if (type == 0xe) {
                if (docked) {
                    Agent *ag = (Agent *)((Mission *)(m))->getAgent();
                    if (Station_getIndex(self->station) == ((Agent *)(ag))->getStation()) {
                        if (Item::isInList(0x73, (ItemArray *)((Ship *)(self->ship))->getCargo()) != 0) return m;
                    }
                }
            } else if (type == 0xb || type == 0) {
                if (docked && Station_getIndex(self->station) == ((Mission *)(m))->getTargetStation()) return m;
            }
            break;
        }
    }
    return (Mission *)0;
}

// Reads the agent roster from disk via a transient FileRead helper.
void Status::loadAgents() {
    FileRead *fr = (FileRead *)operator new(1);
    FileRead_ctor(fr);
    agents = fr->loadAgents();
    FileRead_dtor(fr);
    operator_delete_tail(fr);
}

__attribute__((visibility("hidden"))) extern int g_levelXPTable[];

// Recomputes the player level from the weighted experience sum against the XP threshold table.
void Status::checkForLevelUp() {
    int d4 = __aeabi_idiv(field_d4, 3);
    int a0d = __aeabi_idiv(field_a0, 0x32);
    int sum = a0d + (kills + d4) + field_a4 + missionCount * 2 + currentCampaignMission + stationsVisited;
    for (int i = 0; i != 0x15; i = i + 1) {
        if (sum >= g_levelXPTable[i]) {
            level = i;
        }
    }
}

struct SystemPathFinder;

__attribute__((visibility("hidden"))) extern Status **g_mwStatus;
__attribute__((visibility("hidden"))) extern Station **g_mwPrevStation;

extern "C" {
int Wanted_isActive_mw(Wanted *w);
int Wanted_isTerminated_mw(Wanted *w);
int Status_inAlienOrbit_mw();
int Wanted_getCurrentLocation_mw(Wanted *w);
int Station_getIndex(Station *s);
void FileRead_ctor(FileRead *fr);
void *FileRead_loadSystemsBinary_mw(FileRead *fr);
void *FileRead_dtor_mw(FileRead *fr);
void SystemPathFinder_ctor(SystemPathFinder *p);
void *SystemPathFinder_dtor(SystemPathFinder *p);
void *FileRead_loadStation_mw(int sysAndIndex);
int Station_getSystem_mw(void *s);
int Wanted_getTravelsTo_mw(Wanted *w);
void Wanted_setLastSeen_mw(Wanted *w, int loc);
void Wanted_setCurrentLocation_mw(Wanted *w, int loc);
void Wanted_setTravelsTo_mw(Wanted *w, int loc);
void *Station_dtor_mw(void *s);
void *Globals_getRandomStation_mw();
int SolarSystem_getRoutes_mw();
int SolarSystem_getWarpGateIndex_mw(void *ss);
void ArrayReleaseClasses_SolarSystem(void *a);
void *Array_SolarSystem_dtor(void *a);                               // ... part of dtor chain
void *Array_int_dtor_mw(void *a);
int aeabi_idiv_mw(int a, int b);
int aeabi_idivmod_mw(int a, int b);
void Status_moveWantedTail();                                        // tail 0x1ab098
}

// Status::moveWanted()
//   Advances each active, non-terminated wanted criminal one hop along a pathfound route toward
//   its destination (subject to system-visibility and forbidden-system rules), updating its
//   current location / travels-to / last-seen state.
void Status::moveWanted()
{
    Status *self = this;
    bool loaded = false;
    void *systemsTable = 0;
    SystemPathFinder *pf = 0;
    Status **statusHolder = g_mwStatus;
    Station **prevHolder = g_mwPrevStation;

    for (unsigned i = 0; i < *(unsigned *)self->wanted; i = i + 1) {
        Wanted *w = ((Wanted **)(*(char **)((char *)self->wanted + 4)))[i];
        if (Wanted_isActive_mw(w) == 0)
            continue;
        if (Wanted_isTerminated_mw((*self->wanted)[i]) != 0)
            continue;
        if (Status_inAlienOrbit_mw() != 0)
            continue;

        int loc = Wanted_getCurrentLocation_mw((*self->wanted)[i]);
        if (loc == Station_getIndex((*statusHolder)->station))
            continue;
        if (*prevHolder != 0) {
            if (Station_getIndex(*prevHolder) ==
                Wanted_getCurrentLocation_mw((*self->wanted)[i]))
                continue;
        }

        // lazily load the systems table + pathfinder on first need.
        if (!loaded) {
            FileRead fr;
            FileRead_ctor(&fr);
            systemsTable = FileRead_loadSystemsBinary_mw(&fr);
            ::operator delete(FileRead_dtor_mw(&fr));
            pf = (SystemPathFinder *)operator new(1);
            SystemPathFinder_ctor(pf);
            loaded = true;
        }

        // resolve the from/to stations.
        Wanted_getCurrentLocation_mw((*self->wanted)[i]);
        void *fromSt = FileRead_loadStation_mw(0);
        int fromSys = Station_getSystem_mw(fromSt);
        Wanted_getTravelsTo_mw((*self->wanted)[i]);
        void *toSt = FileRead_loadStation_mw(0);
        int toSys = Station_getSystem_mw(toSt);

        void *path = 0;
        if (Station_getIndex((Station *)fromSt) == Station_getIndex((Station *)toSt)) {
            // already at destination system: re-roll a random reachable target.
            unsigned lo, hi;
            if (i < 2) { hi = 4; lo = 2; }
            else { int r = aeabi_idivmod_mw(i - 1, 6); lo = aeabi_idiv_mw(r, 3) + 2; hi = r / 2 + 4; }
            Wanted *pw = (*self->wanted)[i];
            Wanted_setLastSeen_mw(pw, Wanted_getCurrentLocation_mw(pw));
            if (toSt != 0) ::operator delete(Station_dtor_mw(toSt));
            toSt = Globals_getRandomStation_mw();
            toSys = Station_getSystem_mw(toSt);
            for (;;) {
                path = ((SystemPathFinder *)(pf))->getSystemPath(systemsTable, fromSys, toSys);
                Station_getSystem_mw(toSt);
                int routes = SolarSystem_getRoutes_mw();
                int dsys = Station_getSystem_mw(toSt);
                bool ok = path != 0 && routes != 0 &&
                          *(unsigned *)path >= lo && *(unsigned *)path <= hi &&
                          (*(*statusHolder)->systemVisibilities)[dsys] != 0 &&
                          dsys != 0x1b && dsys != 0x1c && dsys != 0x19 && dsys != 0x1a && dsys != 6 &&
                          dsys != Station_getSystem_mw(fromSt);
                if (ok) {
                    Wanted_setTravelsTo_mw((*self->wanted)[i],
                                           Station_getIndex((Station *)toSt));
                    break;
                }
                if (toSt != 0) ::operator delete(Station_dtor_mw(toSt));
                toSt = Globals_getRandomStation_mw();
                toSys = Station_getSystem_mw(toSt);
            }
        } else if (fromSys == toSys) {
            Wanted *pw = (*self->wanted)[i];
            Wanted_setCurrentLocation_mw(pw, Wanted_getTravelsTo_mw(pw));
            path = 0;
        } else {
            path = ((SystemPathFinder *)(pf))->getSystemPath(systemsTable, fromSys, toSys);
            int wg = SolarSystem_getWarpGateIndex_mw(
                *(void **)(*(int *)((char *)systemsTable + 4) + *(int *)(*(int *)((char *)path + 4) + 4) * 4));
            Wanted_setCurrentLocation_mw((*self->wanted)[i], wg);
        }

        if (toSt != 0) ::operator delete(Station_dtor_mw(toSt));
        if (fromSt != 0) ::operator delete(Station_dtor_mw(fromSt));
        if (path != 0) ::operator delete(Array_int_dtor_mw(path));
    }

    if (systemsTable != 0) {
        ArrayReleaseClasses_SolarSystem(systemsTable);
        ::operator delete(Array_SolarSystem_dtor(systemsTable));
    }
    if (pf != 0) {
        SystemPathFinder_dtor(pf);
        Status_moveWantedTail();
    }
}

// Replaces the wingmen roster with deep copies of the strings in `list` (or clears it).
void Status::setWingmen(Array<String *> *list) {
    Array<String *> *cur = (Array<String *> *)(void *)wingmen;
    if (cur != 0) {
        ArrayReleaseClasses_String(cur);
    }
    if (list == 0) {
        wingmen = 0;
        this->field_0x28 = 0;
        this->field_0x30 = 0;
    } else {
        Array<String *> *na = (Array<String *> *)operator new(0xc);
        Array_String_ctor(na);
        wingmen = (int32_t)(intptr_t)na;
        ArraySetLength_String(list->size(), na);
        for (unsigned i = 0; i < list->size(); i = i + 1) {
            String *s = (String *)operator new(0xc);
            ((String *)(s))->ctor_copy((*list)[i], false);
            (*((Array<String *> *)(void *)wingmen))[i] = s;
        }
    }
}
