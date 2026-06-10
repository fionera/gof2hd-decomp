#ifndef GOF2_STATUS_H
#define GOF2_STATUS_H
#include "gof2/common.h"
// real struct kept from byte-match recovery
struct Status {
    Array<Wanted *> *wanted;               // 0x000
    int32_t collectedBounties[4];          // 0x004 .. 0x010  (getCollectedBounties: this+idx*4+4)
    int32_t standing;                      // 0x014
    int32_t bluePrints;                    // 0x018  (Array<BluePrint*>*)
    int32_t pendingProducts;               // 0x01c
    Array<Agent *> *agents;                // 0x020
    int32_t wingmen;                       // 0x024
    uint8_t pad_028[0x0c];                 // 0x028 .. 0x033
    int32_t passengers;                    // 0x034
    Array<bool> *systemVisibilities;       // 0x038
    uint8_t pad_03c[0x10];                 // 0x03c .. 0x04b
    Array<bool> *field_4c;                 // 0x04c
    Array<bool> *field_50;                 // 0x050
    Array<bool> *field_54;                 // 0x054
    Array<bool> *field_58;                 // 0x058
    uint8_t pad_05c[0x1c];                 // 0x05c .. 0x077
    Station *playerStation;                // 0x078
    uint8_t pad_07c[0x10];                 // 0x07c .. 0x08b
    int32_t field_8c;                      // 0x08c
    int32_t field_90;                      // 0x090  (Array<int>*)
    Array<bool> *field_94;                 // 0x094
    Array<bool> *field_98;                 // 0x098
    int32_t field_9c;                      // 0x09c
    int32_t field_a0;                      // 0x0a0
    int32_t field_a4;                      // 0x0a4
    uint8_t pad_0a8[0x04];                 // 0x0a8 .. 0x0ab
    Array<bool> *field_ac;                 // 0x0ac
    uint8_t pad_0b0[0x04];                 // 0x0b0 .. 0x0b3
    Array<bool> *field_b4;                 // 0x0b4
    uint8_t pad_0b8[0x1c];                 // 0x0b8 .. 0x0d3
    int32_t field_d4;                      // 0x0d4
    uint8_t pad_0d8[0x34];                 // 0x0d8 .. 0x10b
    int32_t field_10c;                     // 0x10c
    int16_t field_110;                     // 0x110
    uint8_t pad_112[0x36];                 // 0x112 .. 0x147
    int32_t field_148;                     // 0x148
    int32_t field_14c;                     // 0x14c
    uint8_t pad_150[0x18];                 // 0x150 .. 0x167
    String string_168;                     // 0x168 .. 0x173
    int32_t field_174;                     // 0x174
    int32_t field_178;                     // 0x178
    uint8_t pad_17c[0x14];                 // 0x17c .. 0x18f
    Ship *ship;                            // 0x190
    Mission *mission;                      // 0x194
    Array<Mission *> *missions;            // 0x198
    Station *station;                      // 0x19c
    Array<Station *> *stationStack;        // 0x1a0
    int32_t system;                        // 0x1a4
    int32_t planetNames;                   // 0x1a8
    int32_t planetTextures;                // 0x1ac
    int32_t credits;                       // 0x1b0
    int32_t rating;                        // 0x1b4
    int64_t playingTime;                   // 0x1b8
    int32_t kills;                         // 0x1c0
    int32_t missionCount;                  // 0x1c4
    int32_t level;                         // 0x1c8
    int32_t lastXP;                        // 0x1cc
    int32_t stationsVisited;               // 0x1d0
    int32_t goodsProduced;                 // 0x1d4
    int32_t pirateKills;                   // 0x1d8
    int32_t jumpgatesUsed;                  // 0x1dc
    int32_t capturedCrates;                // 0x1e0
    int32_t boughtEquipment;               // 0x1e4
    int32_t currentCampaignMission;        // 0x1e8
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
    void departStation();
    bool inAlienOrbit();
    Station *getStation();
    int64_t getPlayingTime();
    void setMission(Mission *mission);
    Ship *getShip();
    bool gameWon();
    int getCurrentCampaignMission();
    int getCampaignMission();
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
    int getSystem();
    bool inFogSkyboxOrbit();
    bool inSupernovaOrbit();
    bool inDeepScienceOrbit();
    bool inBlackMarketSystem();
    bool inPirateLootOrbit();
    bool hardCoreMode();
    Wanted *getWantedInCurrentOrbit();
    int missionFailed(bool param_1, int64_t time);
    void missionCompleted(Mission *mission);
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
#endif
