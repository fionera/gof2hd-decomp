#ifndef GOF2_GUN_H
#define GOF2_GUN_H
#include <cstddef>
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "engine/render/Sparks.h"

#include "game/ItemSort.h"

#include "engine/math/Vector.h"

#include "engine/math/Matrix.h"

class Player;

class Level;
class Sparks;

class Gun {
public:
    int lastHitKIPlayer;
    Player *owner;
    unsigned count;
    char *positions;
    int directionCount;
    char *velocities;
    uint8_t _pad_0x18[8]; // drift: 8 bytes missing upstream (two 4-byte fields) so field_0x20 lands at 0x20
    int field_0x20;
    char *upVectors;
    uint8_t _pad_0x28[4]; // drift: 4 bytes missing so field_0x2c lands at 0x2c
    int field_0x2c;
    char *hitPositions;
    int level;
    int *lifetimes;
    uint8_t *hitFlags;
    int initialLifetime;
    int fireDelay;
    uint8_t active;
    uint8_t hitSmall;

    float pitchRate;

    uint8_t _pad_0x50[4]; // drift: 4 bytes missing so field_0x54 lands at 0x54
    uint8_t field_0x54;
    int itemIndex;
    ItemSort weaponType;
    int damage;
    int empDamage;
    uint8_t useCustomRadius;
    int timer;
    int delayTimer;
    int ammoCount;
    int field_0x78;
    Vector offset;
    uint8_t ignited;
    char field_0x89;
    int32_t field_0x8c;
    int field_0x90;
    int field_0x94;
    int field_0x98;
    int fireIndex;

    uint8_t _pad_0xa0[4]; // drift: 4 bytes missing so the former-union run lands at 0xa4
    // Former union { int field_0xa4; struct{...}; } at 0xa4: the wide int
    // `field_0xa4` is reinterpret_cast<int&>(field_0xa4_b0) at its use sites.
    uint8_t field_0xa4_b0;
    uint8_t field_0xa5;
    uint8_t field_0xa6;
    uint8_t field_0xa7;

    uint8_t field_0xa8;
    uint8_t delayActive;
    Array<int> *wobbleOffsets;
    float field_0xb0;
    Array<Player *> *enemies;
    Sparks *impact;
    Player *target;
    Vector basePos;
    int field_0xcc;
    int field_0xd0;
    int field_0xd4;
    Vector targetDir;
    Vector velocity;
    uint8_t playerGun;
    int slotIndex;
    uint8_t levelCollision;
    uint8_t friendGun;
    float errorMagnitudePercentage;
    int magnitude;
    int customRadius;
    uint8_t homing;
    int *geometries;
    uint8_t *randomFlags;

    Gun(int kind, int p2, int count, int p4, int p5, int p6, float p7, Vector dir, Vector vel);

    ~Gun() noexcept(false);

    void calcCharacterCollision();

    void calcLevelCollision();

    void *getEnemies(); // lint: void_ptr exported method return type; consumed as void* in RocketGun.cpp

    int getMagnitude();

    void ignite();

    uint8_t isPlayerGun();

    void removeAllEnemies();

    void render();

    void setEnemies(Array<Player *> *enemies);

    void setEnemy(Player *enemy);

    void setErrorMagnitudePercentage(int v);

    void setFriendGun(bool v);

    void setLevel(Level *lvl);

    void setImpact(Sparks *impact);

    void setIndex(int index);

    void setLevelCollision(bool v);

    void setMagnitude(int v);

    void setOffset(Vector *v);

    void setOffset(int a, int b);

    void setPlayerGun(bool v);

    void shoot(Matrix m, int n, bool b);

    void shootAt(Matrix m, int n, Player *p, bool b);

    void translate(const Vector &v);

    void update(int dt);
};

#if __SIZEOF_POINTER__ == 4
// Drift fixed: 0x14 bytes of padding inserted upstream (8 @0x18, 4 @0x28, 4 @0x50, 4 @0xa0) so
// every field_0xNN now lands at its named offset 0xNN. The former-union run (a4_b0/a5/a6/a7)
// stays contiguous at 0xa4. Sentinel: targetDir is the first field after the drift run.
static_assert(offsetof(Gun, field_0x20) == 0x20, "");
static_assert(offsetof(Gun, field_0x2c) == 0x2c, "");
static_assert(offsetof(Gun, field_0x54) == 0x54, "");
static_assert(offsetof(Gun, field_0x78) == 0x78, "");
static_assert(offsetof(Gun, field_0x89) == 0x89, "");
static_assert(offsetof(Gun, field_0x8c) == 0x8c, "");
static_assert(offsetof(Gun, field_0x90) == 0x90, "");
static_assert(offsetof(Gun, field_0x94) == 0x94, "");
static_assert(offsetof(Gun, field_0x98) == 0x98, "");
static_assert(offsetof(Gun, field_0xa4_b0) == 0xa4, "Gun former-union byte at 0xa4");
static_assert(offsetof(Gun, field_0xa5) == 0xa5, "");
static_assert(offsetof(Gun, field_0xa6) == 0xa6, "");
static_assert(offsetof(Gun, field_0xa7) == 0xa7, "");
static_assert(offsetof(Gun, field_0xa8) == 0xa8, "");
static_assert(offsetof(Gun, field_0xb0) == 0xb0, "");
static_assert(offsetof(Gun, field_0xcc) == 0xcc, "");
static_assert(offsetof(Gun, field_0xd0) == 0xd0, "");
static_assert(offsetof(Gun, field_0xd4) == 0xd4, "");
static_assert(offsetof(Gun, targetDir) == 0xd8, "Gun first field after drift run");
#endif
#endif
