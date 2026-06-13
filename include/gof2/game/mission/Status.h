#ifndef GOF2_STATUS_H
#define GOF2_STATUS_H
#include "gof2/common.h"
// real struct kept from byte-match recovery (+ supporting decls)
using uint = uint32_t;
using uchar = uint8_t;
using ushort = uint16_t;
using longlong = int64_t;
using ulonglong = uint64_t;

namespace AbyssEngine {

// AbyssEngine::String — 0xc bytes: { uint16_t* text; uint32_t field_04; uint32_t size; }

} // namespace AbyssEngine

using String = AbyssEngine::String;

// Engine container: { uint32_t size; T* data; uint32_t size2; } — 0xc bytes.

// Forward declarations of related game classes (opaque — accessed only via extern "C" helpers).
struct Mission;
struct Station;
struct Ship;
struct Wanted;
struct Agent;
struct BluePrint;
struct Item;
struct PendingProduct;

// =====================================================================================
// Status — the persistent player/game state.  Field offsets recovered from the target.
// =====================================================================================
class Status {
public:
    Array<Wanted *> *wanted;
    int32_t collectedBounties[4];          // 0x004 .. 0x010  (getCollectedBounties: this+idx*4+4)
    int32_t standing;
    Array<BluePrint *> *bluePrints;        // 0x018  (Array<BluePrint*>*)
    Array<PendingProduct *> *pendingProducts; // 0x01c
    Array<Agent *> *agents;
    int32_t wingmen;
    int32_t field_0x28;
    uint8_t pad_02c[0x04];                 // 0x02c .. 0x02f
    int32_t field_0x30;
    int32_t passengers;
    Array<bool> *systemVisibilities;
    Array<int> *field_0x3c;                 // 0x03c  (Array<int>*)
    Array<int> *field_0x40;                 // 0x040  (Array<int>*)
    Array<int> *field_0x44;                 // 0x044  (Array<int>*)
    Array<int> *field_0x48;                 // 0x048  (Array<int>*)
    Array<bool> *field_4c;
    Array<bool> *field_50;
    Array<bool> *field_54;
    Array<bool> *field_58;
    uint8_t pad_05c[0x1c];                 // 0x05c .. 0x077
    Station *playerStation;
    uint8_t pad_07c[0x10];                 // 0x07c .. 0x08b
    int32_t field_8c;
    Array<int> *field_90;                  // 0x090
    Array<bool> *field_94;
    Array<bool> *field_98;
    int32_t field_9c;
    int32_t field_a0;
    int32_t field_a4;
    uint8_t pad_0a8[0x04];                 // 0x0a8 .. 0x0ab
    Array<bool> *field_ac;
    uint8_t pad_0b0[0x04];                 // 0x0b0 .. 0x0b3
    Array<bool> *field_b4;
    uint8_t pad_0b8[0x1c];                 // 0x0b8 .. 0x0d3
    int32_t field_d4;
    uint8_t pad_0d8[0x18];                 // 0x0d8 .. 0x0ef
    uint8_t field_0xf0;
    uint8_t pad_0f1[0x0f];                 // 0x0f1 .. 0x0ff
    int32_t field_0x100;
    int32_t field_0x104;
    uint8_t field_0x108;
    uint8_t pad_109[0x03];                 // 0x109 .. 0x10b
    int32_t field_10c;
    int16_t field_110;
    uint8_t pad_112[0x36];                 // 0x112 .. 0x147
    int32_t field_148;
    int32_t field_14c;
    uint8_t pad_150[0x18];                 // 0x150 .. 0x167
    String string_168;                     // 0x168 .. 0x173
    int32_t field_174;
    int32_t field_178;
    uint8_t field_0x17c;
    uint8_t pad_17d[0x13];                 // 0x17d .. 0x18f
    Ship *ship;
    Mission *mission;
    Array<Mission *> *missions;
    Station *station;
    Array<Station *> *stationStack;
    int32_t system;
    int32_t planetNames;
    int32_t planetTextures;
    int32_t credits;
    int32_t rating;
    int64_t playingTime;
    int32_t kills;
    int32_t missionCount;
    int32_t level;
    int32_t lastXP;
    int32_t stationsVisited;
    int32_t goodsProduced;
    int32_t pirateKills;
    int32_t jumpgatesUsed;
    int32_t capturedCrates;
    int32_t boughtEquipment;
    int32_t currentCampaignMission;
    uint8_t pad_1ec[0x0c];                 // 0x1ec .. 0x1f7

    Status();
    ~Status();

    void addPendingProduct(BluePrint *product);
    int getPendingProducts();
    Array<bool> *getSystemVisibilities();
    void setSystemVisibility(int index, bool value);
    Array<Station *> *getStationStack();
    void setStationStack(Array<Station *> *stack);
    int addStationToStack(Station *station);
    bool isOnStack(Station *station);
    void setStation(Station *station);
    void setStationTail(Station *station);
    void departStation(Station *dest);
    bool inAlienOrbit();
    Station *getStation();
    int64_t getPlayingTime();
    void setMission(Mission *mission);
    Ship *getShip();
    bool gameWon();
    int getCurrentCampaignMission();
    int getCampaignMission();
    // Same slot as getCampaignMission() (missions[0]), but typed as the Mission*
    // it really is. MGame uses it to fetch the active campaign mission object.
    Mission *getCampaignMissionPtr();
    Mission *getMission();
    void moveWanted();
    int getPassengers();
    void setPassengers(int passengers);
    Array<Mission *> *getMissions();
    Mission *getFreelanceMission();
    void setFreelanceMission(Mission *mission);
    void setCampaignMission(Mission *mission);
    int getNumberOfMissions();
    int getMaxMissions();
    void incMissionCount();
    void setCurrentCampaignMission(int value);
    void nextCampaignMission();
    void changeCredits(int delta);
    void setShip(Ship *ship);
    void setStationsVisited(int value);
    int getStationsVisited();
    bool dlc1Won();
    bool inEmptyOrbit();
    uint inPlanetRingOrbit();
    uint orbitHasPlanetRing(int index);
    bool inStormOrbit();
    int inSupernovaSystem();
    static int isChallengeMode();
    // Alien-orbit predicate used by inAlienOrbit(): you are in an "alien" (foreign)
    // orbit whenever the station you are currently orbiting is not your own
    // player/home station. Modeled as a static helper so the heavily-shared call
    // sites (Hud, Galaxy, Level, departStation) all reach the same body.
    static bool inAlienOrbit_impl(Station *station, Station *playerStation);
    int getSystem();
    bool inFogSkyboxOrbit();
    bool inSupernovaOrbit();
    bool inDeepScienceOrbit();
    bool inBlackMarketSystem();
    bool inPirateLootOrbit();
    bool hardCoreMode();
    Wanted *getWantedInCurrentOrbit();
    int missionFailed(bool param_1, int64_t time);
    Mission * missionCompleted(bool atStation, bool docked, long long extra);
    void setJumpgateUsed(int value);
    void jumpgateUsed();
    int getJumpgateUsed();
    void crateCaptured(int delta);
    void setCapturedCrates(int value);
    int getCapturedCrates();
    void incEquipmentBought();
    void setBoughtEquipment(int value);
    int getBoughtEquipment();
    void removeMission(Mission *mission);
    void visitStation();
    int getPlanetNames();
    int getPlanetTextures();
    int getCredits();
    void setRating(int value);
    void setPlayingTime(int64_t value);
    void setKills(int value);
    void setMissionCount(int value);
    void setLevel(int value);
    void setLastXP(int value);
    void setGoodsProduced(int value);
    int getGoodsProduced();
    void incGoodsProduced(int delta);
    void setCredits(int value);
    void checkForLevelUp();
    int getRating();
    int getLastXP();
    void changeRating(int delta);
    int getKills();
    void incKills();
    void setPirateKills(int value);
    int getPirateKills();
    void incPirateKills();
    void addKills(int delta);
    int getMissionCount();
    int getLevel();
    int getStanding();
    int getBluePrints();
    void unlockBluePrint(int index);
    int isBlueprintUnlocked(int index);
    int getAgents();
    void incPlayingTime(int64_t delta);
    int getWingmen();
    void setWingmen(Array<String *> *list);
    bool stringHasToken(String haystack, String needle);
    String replaceHash(String haystack, String needle);
    String replaceHash(String haystack, String needle, String replacement);
    void calcCargoPrices();
    void loadAgents();
    int getCollectedBounties(int index);
    void incCollectedBounties(int index);
    int getGammaRayDamagePerSecond(int param_1, int param_2);
    void loadWanted();
    Array<Wanted *> *getWanted();
    bool isStorylineWanted(int index);
    int wantedBoardAccessible();
    int activateNewWanted();
    void resetGame();

    int isFreighterMissionStation(int station);
    int getFreighterMissionStationBit(int station);
};

// NOTE: the recovered byte-match offset asserts were removed: this native build uses
// natural 64-bit layout (Array<T> == std::vector is 24 bytes, String is backed by
// std::u16string), so the original 32-bit ARM offsets no longer hold. Field NAMES are
// preserved; only the byte-exact layout assumption is dropped.

// Engine globals / runtime allocator hooks.
void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;
#endif
