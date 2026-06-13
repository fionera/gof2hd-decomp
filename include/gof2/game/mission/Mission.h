#ifndef GOF2_MISSION_H
#define GOF2_MISSION_H
#include "gof2/common.h"
#include "gof2/game/world/Station.h"   // provides struct RetStr (single shared definition)
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — Mission class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: namespace). Field offsets recovered from the
// per-method target disassembly. We do NOT model a full layout: methods access
// fields via byte-offset casts from `this` (which arrives in r0 just like the
// first explicit arg of an extern "C" function).
//
// Known offsets:
//   0x00 vtable ptr        0x10 String name        0x1c String targetName
//   0x40 String targetStationName                  0x4c String targetSystemName
//   0x0c int id            0x3c int targetStation  0x58 int reward
//   0x5c bool instantAction 0x60 float distance     0x64 int campaign
//   0x68/0x6c production goods (int,int)            0x74 bool visible


struct Mission;
struct Agent;

// AbyssEngine::String passed by value is a 12-byte aggregate. In the engine it has
// a non-trivial copy ctor/dtor, so by-value params are passed by invisible
// reference (a pointer). Model it opaquely.
using AbyssEngine::String12;

// String returned by value (12-byte aggregate, matches AbyssEngine::String layout).
// Returned through a hidden sret pointer in the decompiled getters below.
// `struct RetStr` is provided by gof2/Station.h (single shared definition).

// Full Mission layout recovered from the per-method disassembly. The four embedded
// String members (0x10/0x1c/0x40/0x4c) are reached via byte-offset casts, so they are
// kept as raw 12-byte slots here.
class Mission {
public:
    void* field_0x0;                    // +0x0  vtable ptr
    int field_0x4;                      // +0x4  status flags: failed (byte +0x4), won (byte +0x5)
    int field_0x8;                      // +0x8  agent pointer slot
    int id;                      // +0xc  id (also reported as "type")
    unsigned char name[12];       // +0x10 String name
    unsigned char targetName[12];       // +0x1c String targetName
    int field_0x28;                     // +0x28 clientImage
    int field_0x2c;                     // +0x2c clientRace
    int field_0x30;                     // +0x30 reward / costs (getCosts reads this slot)
    int field_0x34;                     // +0x34 costs (setCosts writes this slot)
    int field_0x38;                     // +0x38 bonus
    int targetStation;                     // +0x3c targetStation
    unsigned char targetStationName[12];       // +0x40 String targetStationName
    unsigned char targetSystemName[12];       // +0x4c String targetSystemName
    int reward;                     // +0x58 difficulty (getDifficulty reads this slot)
    uint8_t instantAction;                 // +0x5c instantAction
    unsigned char _pad_5d[3];
    int distance;                     // +0x60 distance  getDistance()
    int campaign;                     // +0x64 campaign  setCampaignMission()
    int productionGoodsA;                     // +0x68 production good index
    int productionGoodsB;                     // +0x6c production good amount
    int field_0x70;                     // +0x70 status value
    uint8_t visible;                 // +0x74 visible

    // ---- methods (converted from free functions) ----
    void calcDistance();
    Mission * clone();
    Mission * ctor3(int id, int goods, int station);
    Mission * ctor7(int id, const void *client, int a, int b, int c, int station, int reward);
    Mission * ctor_default();
    Mission * ctor_int(int id);
    void dtor();
    Mission * dtor_inner();
    // Deleting-destructor finisher: free the storage after the inner dtor ran.
    void dtor_finish();
    // Constructor aliases used by the freelance/record/wanted code paths. Each
    // builds the same Mission as the matching numbered ctor; they exist as
    // distinct call-site spellings in the original binary.
    Mission * ctor_akw(int type, int goods, int station);
    Mission * ctorEmpty(int type, int reward, int targetStation);
    Mission * ctorFull(int type, const void *clientName, int *img, int clientRace,
                       int reward, int targetStation, int difficulty);
    void setCampaign_akw(int flag);
    void setWon_akw(int flag);
    RetStr getClientName();
    RetStr getDescription();
    RetStr getName();
    RetStr getTargetName();
    RetStr getTargetStationName();
    RetStr getTargetSystemName();
    bool isCampaignMission();
    bool isEmpty();
    uint8_t isInstantActionMission();
    uint8_t isVisible();
    void setInstantActionMission(bool v);
    void setProductionGoods(int a, int b);
    void * setTargetName(const String12 &rhs);
    void setTargetStation(int idx);
    void * setTargetSystemName(const String12 &rhs);
    void setVisible(bool v);

    // ---- recovered accessors / mutators ----
    int getType();
    void setType(int type);
    int getReward();
    void setReward(int reward);
    int getCosts();
    void setCosts(int costs);
    int getBonus();
    void setBonus(int bonus);
    int getDifficulty();
    int getDistance();
    int getStatusValue();
    void setStatusValue(int value);
    Agent *getAgent();
    void setAgent(Agent *agent);
    int getClientImage();
    int getClientRace();
    int getProductionGoodIndex();
    int getProductionGoodAmount();
    int getTargetStation();
    bool hasFailed();
    void setFailed(bool failed);
    bool hasWon();
    void setWon(bool won);
    void setCampaignMission(bool flag);
    bool isOutsideMission();
};
#endif
