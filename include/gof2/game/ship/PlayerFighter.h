#ifndef GOF2_PLAYERFIGHTER_H
#define GOF2_PLAYERFIGHTER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 - PlayerFighter class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine namespace). Field offsets recovered per-method from
// the target disassembly; we do NOT model a full layout - access fields via byte-offset
// casts from `self`.

struct PlayerFighter;

// AbyssEngine::AEMath::Vector - 3 floats. Passed by const ref / value.
namespace AbyssEngine { namespace AEMath {

} 
    // ---- methods (converted from free functions) ----
    void awake();
    void cloak(int dur, bool b);
    int collide(float x, float y, float z);
    void ctor(int p1, int wingmanCmd, void *player, void *geom, float a, float b, float c, int flag);
    void handleCloaking();
    uint8_t hasCrateCaptured();
    uint8_t hasCrateLost();
    uint8_t hasMissionCrateCaptured();
    uint8_t hasMissionCrateLost();
    void initPush(void *target, int radius);
    int outerCollide(float x, float y, float z);
    void push(int dt);
    void removeTrail();
    void render();
    void roll(int angle);
    void setAIDisabled(bool v);
    void setBV_a(Array<BoundingVolume *> *v);
    void setBV_b(BoundingVolume *bv);
    void setBoostProb(int v);
    void setCloakingPossible(bool v);
    void setExhaustVisible(bool vis);
    void setLevel(Level *lvl);
    void setMissionCrate(bool on);
    void setPosition3(int x, int y, int z);
    void setPosition_ref(const Vector &v);
    void setRotate(int v);
    void setShootError(int v);
    void setSpeed(float v);
    void setWingmanCommand(int cmd, KIPlayer *target);
    void update(int dt);
}
typedef AbyssEngine::AEMath::Vector Vector;

// AbyssEngine::String - 12-byte value aggregate (text*, field, size).
namespace AbyssEngine {

}
typedef AbyssEngine::String String;

// Field accessor via byte offset.

class PlayerFighter {
public:
    int32_t player;                  // +0x4
    int32_t geometry;                  // +0x8
    int32_t subGeometry;                  // +0xc
    uint8_t field_0x24;                 // +0x24
    signed char field_0x25;             // +0x25
    int32_t wingmanCommand;                 // +0x28
    int32_t field_0x38;                 // +0x38
    signed char field_0x43;             // +0x43
    uint8_t field_0x4c;                 // +0x4c
    void* lootList;                   // +0x50
    int32_t level;                 // +0x54
    int32_t posX;                 // +0x58
    int32_t posY;                 // +0x5c
    int32_t posZ;                 // +0x60
    uint8_t missionCrateLost;                 // +0x68
    uint8_t missionCrateCaptured;                 // +0x69
    uint8_t crateLost;                 // +0x6a
    int32_t routeClone;                 // +0x6c
    signed char field_0x74;             // +0x74
    int32_t field_0x78;                 // +0x78
    int32_t field_0x80;                 // +0x80
    int32_t field_0x84;                 // +0x84
    int32_t state;                 // +0x88
    uint8_t isMissionCrate;                 // +0xd0
    int32_t deathTimer;                 // +0xd8
    signed char field_0xdc;             // +0xdc
    int32_t field_0xe4;                 // +0xe4
    int32_t field_0xf0;                 // +0xf0
    uint8_t field_0xf5;                 // +0xf5
    int32_t fov;                 // +0xf8
    signed char rollActive;            // +0x100
    int32_t pushTimer;                // +0x104
    int32_t pushDuration;                // +0x108
    float field_0x11c;                  // +0x11c
    float field_0x120;                  // +0x120
    void* explosion;                  // +0x124
    int32_t field_0x128;                // +0x128
    uint8_t field_0x12c;                // +0x12c
    short field_0x12d;                  // +0x12d
    uint8_t field_0x12e;                // +0x12e
    signed char field_0x12f;            // +0x12f
    int32_t field_0x130;                // +0x130
    int32_t field_0x134;                // +0x134
    int32_t field_0x138;                // +0x138
    signed char field_0x13c;            // +0x13c
    uint8_t field_0x13d;                // +0x13d
    signed char field_0x13e;            // +0x13e
    int32_t field_0x140;                // +0x140
    void* route;                  // +0x144
    int32_t field_0x148;                // +0x148
    int32_t commandRoute;                // +0x14c
    void* boundingVolumes;                  // +0x150
    void* trail;                  // +0x154 Trail*
    int32_t field_0x158;                // +0x158
    int32_t field_0x15c;                // +0x15c
    int32_t field_0x160;                // +0x160
    int32_t field_0x1a0;                // +0x1a0
    float rotate;                  // +0x1a4
    float shootError;                  // +0x1a8
    float speed;                  // +0x1ac
    int32_t field_0x1b0;                // +0x1b0
    int32_t boostProb;                // +0x1b4
    int32_t field_0x1b8;                // +0x1b8
    int32_t field_0x1c0;                // +0x1c0
    int32_t field_0x1c4;                // +0x1c4
    int32_t field_0x1c8;                // +0x1c8
    int32_t deltaTime;                // +0x1d0
    int32_t deltaTimeHi;                // +0x1d4
    int32_t hitpoints;                // +0x1d8
    int32_t field_0x1dc;                // +0x1dc
    signed char field_0x1e0;            // +0x1e0
    int32_t field_0x1e4;                // +0x1e4
    float currentSpeed;                  // +0x1e8
    float currentRotate;                  // +0x1f0
    short field_0x1f4;                  // +0x1f4
    int32_t field_0x1f8;                // +0x1f8
    signed char field_0x1fc;            // +0x1fc
    int32_t field_0x200;                // +0x200
    int32_t field_0x204;                // +0x204
    int32_t field_0x208;                // +0x208
    int32_t field_0x20c;                // +0x20c
    int32_t field_0x210;                // +0x210
    int32_t field_0x214;                // +0x214
    signed char field_0x254;            // +0x254
    signed char field_0x255;            // +0x255
    int32_t field_0x294;                // +0x294
    int32_t field_0x298;                // +0x298
    int32_t field_0x2b0;                // +0x2b0
    signed char field_0x2b4;            // +0x2b4
    void* easeMatrix;                  // +0x2b8
    int32_t field_0x2c4;                // +0x2c4
    int32_t cloakTimer;                // +0x2c8
    int32_t field_0x2c9;                // +0x2c9
    int32_t cloakDuration;                // +0x2cc
    int32_t field_0x2cd;                // +0x2cd
    uint8_t cloakActive;                // +0x2d0
    int32_t cloakCooldown;                // +0x2d4
    uint8_t cloakingPossible;                // +0x2d8
    signed char field_0x2d9;            // +0x2d9
    uint32_t cloakMaterial;               // +0x2dc
    int32_t field_0x2e0;                // +0x2e0
    uint8_t aiDisabled;                // +0x2e4
    int32_t field_0x2e8;                // +0x2e8

    // ---- methods (converted from free functions) ----
    ~PlayerFighter();
    void awake();
    void reset();
    void revive();
    void cloak(int dur, bool b);
    int collide(float x, float y, float z);
    void ctor(int p1, int wingmanCmd, void *player, void *geom, float a, float b, float c, int flag);
    void handleCloaking();
    uint8_t hasCrateCaptured();
    uint8_t hasCrateLost();
    uint8_t hasMissionCrateCaptured();
    uint8_t hasMissionCrateLost();
    void initPush(void *target, int radius);
    int outerCollide(float x, float y, float z);
    void push(int dt);
    void removeTrail();
    void render();
    void roll(int angle);
    void setAIDisabled(bool v);
    void setBV_a(Array<BoundingVolume *> *v);
    void setBV_b(BoundingVolume *bv);
    void setBoostProb(int v);
    void setCloakingPossible(bool v);
    void setExhaustVisible(bool vis);
    void setLevel(Level *lvl);
    void setMissionCrate(bool on);
    void setPosition3(int x, int y, int z);
    void setPosition_ref(const Vector &v);
    void setRotate(int v);
    void setShootError(int v);
    void setSpeed(float v);
    void setWingmanCommand(int cmd, KIPlayer *target);
    void update(int dt);

    // ---- tail-call / veneer fragments ----
    // Each is the terminal b.w of a PlayerFighter method into a relocated GOT slot
    // that lands in the inherited engine/base implementation: AEGeometry's group
    // setter, the awake/exhaust/render dispatch, the bounding-volume array adder,
    // the loot setter and the KIPlayer base destructor. They have no static body of
    // their own (pure GOT veneer), so the work lives behind the extern shim the
    // linker resolves. Modeling them as members keeps the original control flow.
    // The first pointer argument of each veneer is a real parameter (the target
    // geometry / slot / volume / transform), not a `this` receiver, so these are
    // static: they forward straight to the relocated GOT landing pad.
    static void setShipGroup_base(AEGeometry *geom, int group, bool flag);
    static void awake_tail(int geom, int on);
    void cloak_off_helper();
    void *base_dtor();
    static void setMissionCrate_tail(int slot, Array<int> *loot);
    static void setBV_add(BoundingVolume *bv, Array<BoundingVolume *> *volumes);
    static void setExhaustVisible_apply(unsigned int transform, bool visible);
    static void render_tail(int geom);
};
#endif
