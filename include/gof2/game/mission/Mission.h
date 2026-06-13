#ifndef GOF2_MISSION_H
#define GOF2_MISSION_H
#include "gof2/common.h"
#include "gof2/game/world/Station.h"   // provides struct String (single shared definition)
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — Mission class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: namespace). Field offsets recovered from the
// per-method target disassembly. All members are now named and typed; the embedded
// String members are real String fields (auto ctor/dtor).

struct Mission;
struct Agent;

// AbyssEngine::String has a non-trivial copy ctor/dtor, so by-value params are
// passed by invisible reference (a pointer). The canonical std::u16string-backed
// String is defined once in gof2/common.h.
using AbyssEngine::String;

// Full named Mission layout (recovered from the per-method disassembly). The four
// embedded String members (0x10/0x1c/0x40/0x4c) are real String fields: they are
// auto-constructed/destroyed, so the ctors no longer placement-ctor them and ~Mission
// no longer hand-destroys them. Trailing comments are the original 32-bit field
// offsets, kept for cross-reference.
class Mission {
public:
    void* field_0x0;                    // +0x0  vtable ptr
    uint8_t failed;                     // +0x4  failed flag
    uint8_t won;                        // +0x5  won flag
    unsigned char _pad_6[2];            // +0x6
    Agent* agent;                       // +0x8  agent pointer slot
    int id;                             // +0xc  id (also reported as "type")
    String name;                        // +0x10 client name String
    String targetName;                  // +0x1c targetName String
    int field_0x28;                     // +0x28 clientImage
    int field_0x2c;                     // +0x2c clientRace
    int field_0x30;                     // +0x30 reward / costs (getCosts reads this slot)
    int field_0x34;                     // +0x34 costs (setCosts writes this slot)
    int field_0x38;                     // +0x38 bonus
    int targetStation;                  // +0x3c targetStation
    String targetStationName;           // +0x40 targetStationName String
    String targetSystemName;            // +0x4c targetSystemName String
    int reward;                         // +0x58 difficulty (getDifficulty reads this slot)
    uint8_t instantAction;              // +0x5c instantAction
    unsigned char _pad_5d[3];
    int distance;                       // +0x60 distance  getDistance()
    int campaign;                       // +0x64 campaign  setCampaignMission()
    int productionGoodsA;               // +0x68 production good index
    int productionGoodsB;               // +0x6c production good amount
    int field_0x70;                     // +0x70 status value
    uint8_t visible;                    // +0x74 visible

    // ---- constructors / destructor (demangle to Mission::Mission / ~Mission) ----
    // The four String members are real fields, auto ctor/dtor'd.
    Mission();
    Mission(int id);
    Mission(int id, int goods, int station);
    Mission(int id, const void *client, int a, int b, int c, int station, int reward);
    ~Mission();

    // ---- methods (converted from free functions) ----
    void calcDistance();
    Mission * clone();
    void setCampaign_akw(int flag);
    void setWon_akw(int flag);
    String getClientName();
    String getDescription();
    String getName();
    String getTargetName();
    String getTargetStationName();
    String getTargetSystemName();
    bool isCampaignMission();
    bool isEmpty();
    uint8_t isInstantActionMission();
    uint8_t isVisible();
    void setInstantActionMission(bool v);
    void setProductionGoods(int a, int b);
    void * setTargetName(const String &rhs);
    void setTargetStation(int idx);
    void * setTargetSystemName(const String &rhs);
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
