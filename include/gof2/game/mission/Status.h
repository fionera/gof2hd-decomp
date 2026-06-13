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
struct Standing;

// =====================================================================================
// Status — the persistent player/game state.  Field offsets recovered from the target
// binary (android_2.0.16_libgof2hdaa.so).  Members are declared in original-offset order;
// each carries a `// +0xNN` comment giving that recovered 32-bit offset.  The native build
// uses natural 64-bit layout, so the byte-exact spacing no longer holds — only the order
// and the names are load-bearing.
// =====================================================================================
class Status {
public:
    Array<Wanted *> *wanted;                  // +0x000
    int32_t collectedBounties[4];             // +0x004 .. 0x010
    Standing *standing;                       // +0x014  (Standing*; getStanding() returns it as int)
    Array<BluePrint *> *bluePrints;           // +0x018
    Array<PendingProduct *> *pendingProducts; // +0x01c
    Array<Agent *> *agents;                   // +0x020
    int32_t wingmen;                          // +0x024  (holds Array<String*>* as int)
    int32_t field_0x28;                       // +0x028  (wingmen backing buffer)
    int32_t field_0x30;                       // +0x030  (wingmen count)
    int32_t passengers;                       // +0x034
    Array<bool> *systemVisibilities;          // +0x038
    Array<int> *field_0x3c;                   // +0x03c
    Array<int> *field_0x40;                   // +0x040
    Array<int> *field_0x44;                   // +0x044
    Array<int> *field_0x48;                   // +0x048
    Array<bool> *field_4c;                    // +0x04c
    Array<bool> *field_50;                    // +0x050
    Array<bool> *field_54;                    // +0x054
    Array<bool> *field_58;                    // +0x058
    int32_t field_5c;                         // +0x05c  (ship max shield HP)
    int32_t field_60;                         // +0x060  (ship max armor HP)
    int32_t field_64;                         // +0x064  (ship max HP)
    int32_t field_68;                         // +0x068
    int32_t field_6c;                         // +0x06c
    int32_t field_70;                         // +0x070  (last-station playing-time lo)
    int32_t field_74;                         // +0x074  (last-station playing-time hi)
    Station *playerStation;                   // +0x078
    int32_t field_7c;                         // +0x07c  (current/random system index)
    int32_t field_80;                         // +0x080  (target/dest station index)
    int32_t field_84;                         // +0x084
    int32_t field_88;                         // +0x088  (freelance retry counter)
    int32_t field_8c;                         // +0x08c
    Array<int> *field_90;                     // +0x090
    Array<bool> *field_94;                    // +0x094
    Array<bool> *field_98;                    // +0x098
    int32_t field_9c;                         // +0x09c
    int32_t field_a0;                         // +0x0a0
    int32_t field_a4;                         // +0x0a4
    int32_t field_a8;                         // +0x0a8
    Array<bool> *field_ac;                    // +0x0ac
    int32_t field_b0;                         // +0x0b0
    Array<bool> *field_b4;                    // +0x0b4
    int32_t field_b8;                         // +0x0b8
    int32_t field_bc;                         // +0x0bc
    int32_t field_c0;                         // +0x0c0
    int32_t field_c4;                         // +0x0c4
    int32_t field_c8;                         // +0x0c8
    int32_t field_cc;                         // +0x0cc
    int32_t field_d0;                         // +0x0d0
    int32_t field_d4;                         // +0x0d4
    int32_t field_d8;                         // +0x0d8
    int32_t field_dc;                         // +0x0dc
    int32_t field_e0;                         // +0x0e0
    int32_t field_e4;                         // +0x0e4
    int32_t field_e8;                         // +0x0e8
    int32_t field_ec;                         // +0x0ec
    int16_t field_0xf0;                       // +0x0f0
    int32_t field_f4;                         // +0x0f4  (sentinel -1)
    int16_t field_f8;                         // +0x0f8  (defaults to 1)
    int32_t field_0x100;                      // +0x100  (last-position lo, copy of playingTime lo)
    int32_t field_0x104;                      // +0x104  (last-position hi, copy of playingTime hi)
    uint8_t field_0x108;                      // +0x108
    int32_t field_10c;                        // +0x10c
    int16_t field_110;                        // +0x110
    uint8_t field_0x111;                      // +0x111
    int32_t field_114;                        // +0x114  (difficulty/mode: 0 or 3)
    int32_t field_118;                        // +0x118
    int32_t field_11c;                        // +0x11c
    uint8_t field_120;                        // +0x120
    int32_t field_124;                        // +0x124
    uint8_t field_128;                        // +0x128
    int32_t field_12c;                        // +0x12c
    uint8_t field_130;                        // +0x130
    int32_t field_134;                        // +0x134
    uint8_t field_138;                        // +0x138
    int32_t field_13c;                        // +0x13c
    uint8_t field_140;                        // +0x140
    int32_t field_144;                        // +0x144
    uint8_t field_148;                        // +0x148
    int32_t field_14c;                        // +0x14c  (scratch Station*, stored as int)
    int32_t field_150;                        // +0x150  (sentinel -1)
    int32_t field_154;                        // +0x154  (sentinel -1)
    int32_t field_158;                        // +0x158  (sentinel -1)
    int32_t field_160;                        // +0x160
    int32_t field_164;                        // +0x164
    String string_168;                        // +0x168 .. 0x173
    int32_t field_174;                        // +0x174
    int32_t field_178;                        // +0x178
    uint8_t field_0x17c;                      // +0x17c
    Ship *ship;                               // +0x190
    Mission *mission;                         // +0x194
    Array<Mission *> *missions;               // +0x198
    Station *station;                         // +0x19c
    Array<Station *> *stationStack;           // +0x1a0
    int32_t system;                           // +0x1a4
    int32_t planetNames;                      // +0x1a8
    int32_t planetTextures;                   // +0x1ac
    int32_t credits;                          // +0x1b0
    int32_t rating;                           // +0x1b4
    int64_t playingTime;                      // +0x1b8 .. 0x1bf
    int32_t kills;                            // +0x1c0
    int32_t missionCount;                     // +0x1c4
    int32_t level;                            // +0x1c8
    int32_t lastXP;                           // +0x1cc
    int32_t stationsVisited;                  // +0x1d0
    int32_t goodsProduced;                    // +0x1d4
    int32_t pirateKills;                      // +0x1d8
    int32_t jumpgatesUsed;                    // +0x1dc
    int32_t capturedCrates;                   // +0x1e0
    int32_t boughtEquipment;                  // +0x1e4
    int32_t currentCampaignMission;           // +0x1e8

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
