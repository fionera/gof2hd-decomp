#ifndef GOF2_STATUS_H
#define GOF2_STATUS_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

using AbyssEngine::String;

class Mission;
class Station;
class Ship;
class Wanted;
class Agent;
class BluePrint;
class Item;
class PendingProduct;
class Standing;

// Status — the persistent player/game state: the active ship, mission roster,
// station stack, wanted/blueprint/agent lists and all the running campaign
// counters. A single Status instance is the save-game's root object.
class Status {
public:
    Array<Wanted*>* wanted;
    int32_t collectedBounties[4];
    Standing* standing;
    Array<BluePrint*>* bluePrints;
    Array<PendingProduct*>* pendingProducts;
    Array<Agent*>* agents;
    int32_t wingmen;                          // holds Array<String*>* as int
    int32_t field_0x28;                       // wingmen backing buffer
    int32_t field_0x30;                       // wingmen count
    int32_t passengers;
    Array<bool>* systemVisibilities;
    Array<int>* field_0x3c;
    Array<int>* field_0x40;
    Array<int>* field_0x44;
    Array<int>* field_0x48;
    Array<bool>* field_4c;
    Array<bool>* field_50;
    Array<bool>* field_54;
    Array<bool>* field_58;
    int32_t field_5c;                         // ship max shield HP
    int32_t field_60;                         // ship max armor HP
    int32_t field_64;                         // ship max HP
    int32_t field_68;
    int32_t field_6c;
    int32_t field_70;                         // last-station playing-time lo
    int32_t field_74;                         // last-station playing-time hi
    Station* playerStation;
    int32_t field_7c;                         // current/random system index
    int32_t field_80;                         // target/dest station index
    int32_t field_84;
    int32_t field_8c;                         // freelance retry counter
    Array<int>* field_90;
    Array<bool>* field_94;
    Array<bool>* field_98;
    int32_t field_9c;
    int32_t field_a0;
    int32_t field_a4;
    int32_t field_a8;
    Array<bool>* field_ac;
    int32_t field_b0;
    Array<bool>* field_b4;
    int32_t field_b8;
    int32_t field_bc;
    int32_t field_c0;
    int32_t field_c4;
    int32_t field_c8;
    int32_t field_cc;
    int32_t field_d0;
    int32_t field_d4;
    int32_t field_d8;
    int32_t field_dc;
    int32_t field_e0;
    int32_t field_e4;
    int32_t field_e8;
    int32_t field_ec;
    int16_t field_0xf0;
    int32_t field_f4;
    int16_t field_f8;
    int32_t field_0x100;                      // last-position lo, copy of playingTime lo
    int32_t field_0x104;                      // last-position hi, copy of playingTime hi
    uint8_t field_0x108;
    int32_t field_10c;
    int16_t field_110;
    uint8_t field_0x111;
    int32_t field_114;                        // difficulty/mode: 0 or 3
    int32_t field_118;
    int32_t field_11c;
    uint8_t field_120;
    int32_t field_124;
    uint8_t field_128;
    int32_t field_12c;
    uint8_t field_130;
    int32_t field_134;
    uint8_t field_138;
    int32_t field_13c;
    uint8_t field_140;
    int32_t field_144;
    uint8_t field_148;
    union {                                   // +0x14c: per-game scratch "void" Station
        Station* voidStation;
        int32_t field_14c;                    // legacy int view (external readers cast this)
    };
    int32_t field_150;
    int32_t field_154;
    int32_t field_158;
    int32_t field_160;
    int32_t field_164;
    String string_168;
    int32_t field_174;
    int32_t field_178;
    uint8_t field_0x17c;
    Ship* ship;
    Mission* mission;
    Array<Mission*>* missions;
    Station* station;
    Array<Station*>* stationStack;
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

    Status();
    ~Status();

    void addPendingProduct(BluePrint* product);
    int getPendingProducts();
    Array<bool>* getSystemVisibilities();
    void setSystemVisibility(int index, bool value);
    Array<Station*>* getStationStack();
    void setStationStack(Array<Station*>* stack);
    int addStationToStack(Station* station);
    bool isOnStack(Station* station);
    void setStation(Station* station);
    void departStation(Station* dest);
    bool inAlienOrbit();
    Station* getStation();
    int64_t getPlayingTime();
    void setMission(Mission* mission);
    Ship* getShip();
    bool gameWon();
    int getCurrentCampaignMission();
    int getCampaignMission();
    Mission* getMission();
    void moveWanted();
    int getPassengers();
    void setPassengers(int passengers);
    Array<Mission*>* getMissions();
    Mission* getFreelanceMission();
    void setFreelanceMission(Mission* mission);
    void setCampaignMission(Mission* mission);
    int getNumberOfMissions();
    int getMaxMissions();
    void incMissionCount();
    void setCurrentCampaignMission(int value);
    void nextCampaignMission(bool advance);
    void changeCredits(int delta);
    void setShip(Ship* ship);
    void setStationsVisited(int value);
    int getStationsVisited();
    bool dlc1Won();
    bool inEmptyOrbit();
    uint32_t inPlanetRingOrbit();
    uint32_t orbitHasPlanetRing(int index);
    bool inStormOrbit();
    int inSupernovaSystem();
    int getSystem();
    bool inFogSkyboxOrbit();
    bool inSupernovaOrbit();
    bool inDeepScienceOrbit();
    bool inBlackMarketSystem();
    bool inPirateLootOrbit();
    bool hardCoreMode();
    Wanted* getWantedInCurrentOrbit();
    int missionFailed(bool docked, int64_t time);
    Mission* missionCompleted(bool atStation, bool docked, long long extra);
    void setJumpgateUsed(int value);
    void jumpgateUsed();
    int getJumpgateUsed();
    void crateCaptured(int delta);
    void setCapturedCrates(int value);
    int getCapturedCrates();
    void incEquipmentBought();
    void setBoughtEquipment(int value);
    int getBoughtEquipment();
    void removeMission(Mission* mission);
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
    void setWingmen(Array<String*>* list);
    bool stringHasToken(String haystack, String needle);
    String replaceHash(String haystack, String needle);
    String replaceHash(String haystack, String needle, String replacement);
    void calcCargoPrices();
    void loadAgents();
    Array<int>* loadAgents(Array<int>* agents);
    int getCollectedBounties(int index);
    void incCollectedBounties(int index);
    int getGammaRayDamagePerSecond(int station, int system);
    void loadWanted();
    Array<Wanted*>* getWanted();
    bool isStorylineWanted(int index);
    int wantedBoardAccessible();
    int activateNewWanted();
    void resetGame();

    int isFreighterMissionStation(int station);
    int getFreighterMissionStationBit(int station);
};
extern Status* gStatus;            // canonical Status singleton (binary .bss 0x2281b0)

#endif
