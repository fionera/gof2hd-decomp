#ifndef GOF2_AGENT_H
#define GOF2_AGENT_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "game/core/String.h"

// Galaxy on Fire 2 — Agent: an NPC trader / contact met in a space station lounge.
// Holds the agent's identity, the system/station it belongs to, its current offer
// and sell-item data, and up to two wingman friend names.

class Mission;

// Sell-item data triple returned by setSellItemData (index/quantity/price).
struct Triple { int a, b, c; };

class Agent {
public:
    AbyssEngine::String name;
    AbyssEngine::String* wingman1;              // friend-name String* (1st wingman)
    AbyssEngine::String* wingman2;              // friend-name String* (2nd wingman)
    int wingmanCount;
    AbyssEngine::String systemName;
    // Dialogue flags persisted by RecordHandler (read elsewhere as agent->field_0x24/25),
    // matching DeepOpen's wasAskedForDifficulty / wasAskedForLocation booleans.
    union { uint8_t wasAskedForDifficulty; uint8_t field_0x24; };  // +0x24
    union { uint8_t wasAskedForLocation;   uint8_t field_0x25; };  // +0x25
    int field_0x28;                             // +0x28 (write-only cache, inited -1; no reader)
    int field_0x2c;                             // +0x2c (write-only cache, inited -1; no reader)
    int field_0x30;                             // +0x30 (write-only cache, inited -1; no reader)
    int sellItemIndex;
    int sellItemQuantity;
    int sellItemPrice;
    unsigned type;                              // (== index)
    int station;
    int system;
    int race;
    uint8_t male;
    int eventCount;
    int category;
    int offer;
    int costs;
    int sellSystemIndex;
    int sellBlueprintIndex;
    AbyssEngine::String missionString;
    AbyssEngine::String stationName;
    uint8_t offerAccepted;
    uint8_t rewardAtNextChat;
    int* imageParts;
    Mission* mission;
    Array<AbyssEngine::String*>* wingmanNames;
    int sellModIndex;

    Agent(int kind, AbyssEngine::String name, int station, int system, int race,
          bool male, int sellSystemIndex, int sellBlueprintIndex, int sellModIndex,
          int sellItemPrice);
    ~Agent() noexcept(false);

    int* getImageParts();
    Mission* getMission();
    AbyssEngine::String getMissionString();
    int getModPricePercentage();
    AbyssEngine::String getName();
    int getSellModIndex();
    AbyssEngine::String getStationName();
    AbyssEngine::String getSystemName();
    AbyssEngine::String getWingmanName(int idx);
    Array<AbyssEngine::String*>* getWingmanNames();
    void giveRewardAtNextChat(bool v);
    uint8_t hasAcceptedOffer();
    uint8_t hasReward();
    bool isGenericAgent();
    bool isKnown();
    void setKnown(bool known);
    uint8_t isMale();
    bool isStoryAgent();
    void nextEvent();
    void setImageParts(int* parts);
    void setMission(Mission* mission);
    void setMissionString(AbyssEngine::String src);
    void setOfferAccepted(bool v);
    Triple* setSellItemData(int index, int quantity, int price);
    void setStationName(AbyssEngine::String src);
    void setSystemName(AbyssEngine::String src);
    void setWingmanFriendNames(Array<AbyssEngine::String*>* param);

    // ---- simple field accessors ----
    int getStation();
    int getSystem();
    int getRace();
    int getIndex();
    int getCosts();
    void setCosts(int costs);
    int getOffer();
    void setOffer(int offer);
    int getEvent();
    void setEvent(int event);
    int getSellItemIndex();
    int getSellItemQuantity();
    int getSellItemPrice();
    void setSellItemPrice(int price);
    int getSellSystemIndex();
    int getSellBlueprintIndex();
    int getWingmanFriendsCount();

    // Tail of setWingmanFriendNames: free the consumed source array's backing.
    void finishWingman(Array<AbyssEngine::String*> *consumedArray);
};
#endif
