#ifndef GOF2_STATUS_H
#define GOF2_STATUS_H
#include <cstddef>
#include "BluePrint.h"
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "Mission.h"
#include "PendingProduct.h"
#include "game/ship/Agent.h"
#include "game/ship/Ship.h"
#include "game/world/Standing.h"
#include "game/world/Station.h"
#include "game/world/Wanted.h"

class Agent;
class BluePrint;
class Mission;
class PendingProduct;
class Ship;
class Standing;
class Station;
class Wanted;

using AbyssEngine::String;

#pragma pack(push, 4)
class Status {
public:
    Array<Wanted *> *wanted;
    int32_t collectedBounties[4];
    Standing *standing;
    Array<BluePrint *> *bluePrints;
    Array<PendingProduct *> *pendingProducts;
    Array<Agent *> *agents;
    int32_t wingmen;

    union { // lint: union_decl -- 32-bit word overlaid with per-byte access
        int32_t field_0x28;

        struct {
            uint8_t _byte_0x28;
            uint8_t _byte_0x29;
            uint8_t byte_0x2a;
            uint8_t _byte_0x2b;
        };
    };

    int32_t field_0x2c;

    uint32_t fadeValue;

    union { // lint: union_decl -- int overlaid with per-byte access
        int32_t passengers;

        struct {
            uint8_t _byte_0x34;
            uint8_t byte_0x35;
            uint8_t _byte_0x36;
            uint8_t byte_0x37;
        };
    };

    union { // lint: union_decl -- Array<bool>* overlaid with flag bytes
        Array<bool> *systemVisibilities;

        struct {
            uint8_t field_0x34_b0;

            uint8_t dlcOverrideFlag;

            uint8_t field_0x36_b2;

            uint8_t versionOverrideFlag;
        };
    };

    Array<int> *field_0x3c;
    Array<int> *field_0x40;
    Array<int> *field_0x44;
    Array<int> *field_0x48;

    union { // lint: union_decl -- Array<bool>* overlaid with per-byte access
        Array<bool> *field_4c;

        struct {
            uint8_t _byte_0x4c;
            uint8_t _byte_0x4d;
            uint8_t byte_0x4e;
            uint8_t _byte_0x4f;
        };
    };

    Array<bool> *field_50;
    Array<bool> *field_54;
    Array<bool> *field_58;
    int32_t field_5c;
    int32_t field_60;
    int32_t field_64;
    int32_t field_68;
    int32_t field_6c;
    int32_t field_70;
    int32_t field_74;
    Station *playerStation;
    int32_t field_7c;
    int32_t field_80;
    int32_t field_84;
    int32_t field_88;

    int32_t field_8c;

    Array<int> *field_90;
    Array<bool> *field_94;
    Array<bool> *field_98;
    int32_t field_9c;
    int32_t field_a0;
    int32_t field_a4;
    int32_t field_a8;
    Array<bool> *field_ac;
    int32_t field_b0;
    Array<bool> *field_b4;
    int32_t field_b8;
    int32_t field_bc;
    int32_t field_c0;
    int32_t field_c4;

    union { // lint: union_decl -- two 32-bit fields overlaid with 64-bit access
        struct {
            int32_t field_c8;
            int32_t field_cc;
        };

        int64_t field_c8_q;
    };

    int32_t field_d0;
    int32_t field_d4;
    int32_t field_d8;
    int32_t field_dc;
    int32_t field_e0;
    int32_t field_e4;
    int32_t field_e8;
    int32_t field_ec;
    uint8_t field_0xf0;
    uint8_t field_0xf1;
    uint8_t _pad_0xf2[2];
    int32_t field_f4;
    int16_t field_f8;
    uint8_t _pad_0xfa[6];
    int32_t field_0x100;
    int32_t field_0x104;
    uint8_t field_0x108;
    uint8_t _pad_0x109[3];

    union { // lint: union_decl -- struct overlaid with 64-bit access
        struct {
            int32_t field_10c;
            uint8_t field_110;
            uint8_t field_0x111;
            uint8_t _pad_0x112[2];
        };

        int64_t field_10c_q;
    };

    int32_t field_114;
    int32_t field_118;
    int32_t field_11c;

    union { // lint: union_decl -- uint8 field_120 vs int32 mode_0x114, both used cross-file
        uint8_t field_120;
        int32_t mode_0x114;
    };

    int32_t field_124;
    int32_t field_128;
    int32_t field_12c;
    int32_t field_130;
    int32_t field_134;
    int32_t field_138;
    int32_t field_13c;
    int32_t field_140;
    int32_t field_144;
    int32_t field_148;

    union { // lint: union_decl -- Station* vs int32 handle, both used cross-file (Generator.cpp field_14c)
        Station *voidStation;
        int32_t field_14c;
    };

    int32_t field_150;
    int32_t field_154;
    int32_t field_158;
    int32_t field_0x15c;
    int32_t field_160;
    int32_t field_164;
    String string_168;
    int32_t field_174;
    int32_t field_178;
    uint8_t field_0x17c;
    uint8_t _pad_0x17d[7];
    int32_t field_0x184;
    int32_t field_0x188;
    int32_t field_0x18c;
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

    void departStation(Station *dest);

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

    void nextCampaignMission(bool advance);

    void changeCredits(int delta);

    void setShip(Ship *ship);

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

    Wanted *getWantedInCurrentOrbit();

    int missionFailed(bool docked, int64_t time);

    Mission *missionCompleted(bool atStation, bool docked, long long extra);

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

    Array<int> *loadAgents(Array<int> *agents);

    int getCollectedBounties(int index);

    void incCollectedBounties(int index);

    int getGammaRayDamagePerSecond(int station, int system);

    void loadWanted();

    Array<Wanted *> *getWanted();

    bool isStorylineWanted(int index);

    int wantedBoardAccessible();

    int activateNewWanted();

    void resetGame();

    int isFreighterMissionStation(int station);

    int getFreighterMissionStationBit(int station);
};
#pragma pack(pop)

#if __SIZEOF_POINTER__ == 4
static_assert(sizeof(Status) == 492, "Status must be 492 bytes");
static_assert(offsetof(Status, wanted) == 0x0, "");
static_assert(offsetof(Status, standing) == 0x14, "");
static_assert(offsetof(Status, bluePrints) == 0x18, "");
static_assert(offsetof(Status, pendingProducts) == 0x1c, "");
static_assert(offsetof(Status, agents) == 0x20, "");
static_assert(offsetof(Status, field_0x28) == 0x28, "");
static_assert(offsetof(Status, fadeValue) == 0x30, "");
static_assert(offsetof(Status, passengers) == 0x34, "");
static_assert(offsetof(Status, systemVisibilities) == 0x38, "");
static_assert(offsetof(Status, field_5c) == 0x5c, "shieldHp");
static_assert(offsetof(Status, field_60) == 0x60, "armorHp");
static_assert(offsetof(Status, field_64) == 0x64, "hullHp");
static_assert(offsetof(Status, field_68) == 0x68, "energy");
static_assert(offsetof(Status, playerStation) == 0x78, "");
static_assert(offsetof(Status, field_8c) == 0x8c, "previousShip");
static_assert(offsetof(Status, field_0xf0) == 0xf0, "");
static_assert(offsetof(Status, field_f4) == 0xf4, "");
static_assert(offsetof(Status, field_0x100) == 0x100, "");
static_assert(offsetof(Status, field_110) == 0x110, "");
static_assert(offsetof(Status, field_0x111) == 0x111, "");
static_assert(offsetof(Status, field_114) == 0x114, "");
static_assert(offsetof(Status, field_11c) == 0x11c, "");
static_assert(offsetof(Status, field_124) == 0x124, "");
static_assert(offsetof(Status, field_14c) == 0x14c, "generatedStation");
static_assert(offsetof(Status, field_150) == 0x150, "");
static_assert(offsetof(Status, field_158) == 0x158, "");
static_assert(offsetof(Status, field_178) == 0x178, "missionCounter");
static_assert(offsetof(Status, field_0x17c) == 0x17c, "");
static_assert(offsetof(Status, ship) == 0x190, "");
static_assert(offsetof(Status, mission) == 0x194, "campaignMission");
static_assert(offsetof(Status, missions) == 0x198, "");
static_assert(offsetof(Status, station) == 0x19c, "currentStation");
static_assert(offsetof(Status, stationStack) == 0x1a0, "");
static_assert(offsetof(Status, system) == 0x1a4, "");
static_assert(offsetof(Status, credits) == 0x1b0, "money");
static_assert(offsetof(Status, rating) == 0x1b4, "");
static_assert(offsetof(Status, kills) == 0x1c0, "");
static_assert(offsetof(Status, missionCount) == 0x1c4, "");
static_assert(offsetof(Status, level) == 0x1c8, "");
static_assert(offsetof(Status, lastXP) == 0x1cc, "");
static_assert(offsetof(Status, stationsVisited) == 0x1d0, "");
static_assert(offsetof(Status, goodsProduced) == 0x1d4, "");
static_assert(offsetof(Status, jumpgatesUsed) == 0x1dc, "");
static_assert(offsetof(Status, capturedCrates) == 0x1e0, "");
static_assert(offsetof(Status, boughtEquipment) == 0x1e4, "");
static_assert(offsetof(Status, currentCampaignMission) == 0x1e8, "campaignStep");
#endif

#endif
