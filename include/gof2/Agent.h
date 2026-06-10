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
using AbyssEngine::String12;

// String returned by value (12-byte aggregate, matches AbyssEngine::String
// layout). Returned through a hidden sret pointer in the decompiled getters.
struct __attribute__((aligned(4))) RetStr { uint32_t a, b, c; };

// Sell-item data triple returned by setSellItemData (item/price/percentage),
// reached via byte-offset casts at the call sites; modelled as three ints.
struct Triple { int a, b, c; };

// Field accessors via byte offsets.

struct Agent {
    void* field_0xc;                    // +0xc
    void* field_0x10;                   // +0x10
    int field_0x14;                     // +0x14
    unsigned char field_0x24;           // +0x24
    unsigned char field_0x25;           // +0x25
    int field_0x28;                     // +0x28
    int field_0x2c;                     // +0x2c
    int field_0x30;                     // +0x30
    int field_0x34;                     // +0x34
    int field_0x38;                     // +0x38
    int field_0x3c;                     // +0x3c
    unsigned field_0x40;                // +0x40
    int field_0x44;                     // +0x44
    int field_0x48;                     // +0x48
    int field_0x4c;                     // +0x4c
    uint8_t field_0x50;                 // +0x50
    int field_0x54;                     // +0x54
    int field_0x58;                     // +0x58
    int field_0x5c;                     // +0x5c
    int field_0x60;                     // +0x60
    int field_0x64;                     // +0x64
    int field_0x68;                     // +0x68
    uint8_t field_0x84;                 // +0x84
    uint8_t field_0x85;                 // +0x85
    int* field_0x88;                    // +0x88
    Mission* field_0x8c;                // +0x8c
    void* field_0x90;                   // +0x90
    int field_0x94;                     // +0x94

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
};
#endif
