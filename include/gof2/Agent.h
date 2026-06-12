#ifndef GOF2_AGENT_H
#define GOF2_AGENT_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — Agent class (NPC/trader). Top-level, no namespace.
// Field offsets recovered from per-method target disassembly. We do NOT model a
// full layout: methods access fields via byte-offset casts from `this`.


struct Agent;

// AbyssEngine::String passed by value is a 12-byte trivially-copied aggregate
// (text*, size, ...). Model it opaquely so it is passed on the stack/regs the
// same way as the target ABI (struct >8 bytes by value).
// (String12 is provided globally by gof2/common.h)

// RetStr is provided by gof2/common.h.

// Sell-item data triple returned by setSellItemData (item/price/percentage),
// reached via byte-offset casts at the call sites; modelled as three ints.
struct Triple { int a, b, c; };

// Field accessors via byte offsets.

class Agent {
public:
    void* wingman1;                    // +0xc
    void* wingman2;                   // +0x10
    int wingmanCount;                     // +0x14
    unsigned char field_0x24;           // +0x24
    unsigned char field_0x25;           // +0x25
    int field_0x28;                     // +0x28
    int field_0x2c;                     // +0x2c
    int field_0x30;                     // +0x30
    int field_0x34;                     // +0x34
    int field_0x38;                     // +0x38
    int field_0x3c;                     // +0x3c
    unsigned type;                // +0x40
    int field_0x44;                     // +0x44
    int field_0x48;                     // +0x48
    int field_0x4c;                     // +0x4c
    uint8_t male;                 // +0x50
    int eventCount;                     // +0x54
    int category;                     // +0x58
    int field_0x5c;                     // +0x5c
    int field_0x60;                     // +0x60
    int field_0x64;                     // +0x64
    int field_0x68;                     // +0x68
    uint8_t offerAccepted;                 // +0x84
    uint8_t rewardAtNextChat;                 // +0x85
    int* imageParts;                    // +0x88
    Mission* mission;                // +0x8c
    void* wingmanNames;                   // +0x90
    int sellModIndex;                     // +0x94

    // Real C++ destructor so the demangled symbol contains "~Agent".
    ~Agent() noexcept(false);

    // ---- methods (converted from free functions) ----
    Agent * ctor(unsigned kind, void *name, int p4, int p5, int p6, char p7, int p8, int p9, int p10, int p11);
    int * getImageParts();
    Mission * getMission();
    RetStr getMissionString();
    int getModPricePercentage();
    RetStr getName();
    int getSellModIndex();
    RetStr getStationName();
    RetStr getSystemName();
    RetStr getWingmanName(int idx);
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
    void setStationName(String12 src);
    void setSystemName(String12 src);
    void setWingmanFriendNames(uint32_t *param);

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
};
#endif
