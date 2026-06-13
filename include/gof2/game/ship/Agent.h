#ifndef GOF2_AGENT_H
#define GOF2_AGENT_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — Agent class (NPC/trader). Top-level, no namespace.
// Field offsets recovered from per-method target disassembly. We do NOT model a
// full layout: methods access fields via byte-offset casts from `this`.

struct Agent;

// Sell-item data triple returned by setSellItemData (index/quantity/price).
struct Triple { int a, b, c; };

// Full named layout (recovered from the binary). Trailing comments are the original
// 32-bit field offsets, kept for cross-reference; the live layout is natural 64-bit.
class Agent {
public:
    String name;                                 // +0x00
    void* wingman1;                              // +0x0c  friend-name String* (1st wingman)
    void* wingman2;                              // +0x10  friend-name String* (2nd wingman)
    int wingmanCount;                            // +0x14
    String systemName;                           // +0x18
    unsigned char field_0x24;                    // +0x24
    unsigned char field_0x25;                    // +0x25
    int field_0x28;                              // +0x28
    int field_0x2c;                              // +0x2c
    int field_0x30;                              // +0x30
    int sellItemIndex;                           // +0x34
    int sellItemQuantity;                        // +0x38
    int sellItemPrice;                           // +0x3c
    unsigned type;                               // +0x40  (== index)
    int station;                                 // +0x44
    int system;                                  // +0x48
    int race;                                    // +0x4c
    uint8_t male;                                // +0x50
    int eventCount;                              // +0x54
    int category;                                // +0x58
    int offer;                                   // +0x5c
    int costs;                                   // +0x60
    int sellSystemIndex;                         // +0x64
    int sellBlueprintIndex;                      // +0x68
    String missionString;                        // +0x6c
    String stationName;                          // +0x78
    uint8_t offerAccepted;                       // +0x84
    uint8_t rewardAtNextChat;                    // +0x85
    int* imageParts;                             // +0x88
    Mission* mission;                            // +0x8c
    Array<AbyssEngine::String*>* wingmanNames;   // +0x90
    int sellModIndex;                            // +0x94

    // Real C++ constructor/destructor so the demangled symbols read Agent::Agent / ~Agent.
    Agent(unsigned kind, void *name, int p4, int p5, int p6, char p7, int p8, int p9, int p10, int p11);
    ~Agent() noexcept(false);

    // ---- methods (converted from free functions) ----
    int * getImageParts();
    Mission * getMission();
    String getMissionString();
    int getModPricePercentage();
    String getName();
    int getSellModIndex();
    String getStationName();
    String getSystemName();
    String getWingmanName(int idx);
    void * getWingmanNames();
    void giveRewardAtNextChat(bool v);
    uint8_t hasAcceptedOffer();
    uint8_t hasReward();
    bool isGenericAgent();
    bool isKnown();
    void setKnown(bool known);
    uint8_t isMale();
    bool isStoryAgent();
    void nextEvent();
    void setImageParts(int *parts);
    void setMission(Mission *mission);
    void setMissionString(void *src);
    void setOfferAccepted(bool v);
    Triple * setSellItemData(int a, int b, int c);
    void setStationName(String src);
    void setSystemName(String src);
    void setWingmanFriendNames(Array<AbyssEngine::String*> *param);

    // ---- simple field accessors (recovered) ----
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
