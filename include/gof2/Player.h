#ifndef GOF2_PLAYER_H
#define GOF2_PLAYER_H
#include "gof2/common.h"
// real struct kept from byte-match recovery (+ supporting decls)
// Galaxy on Fire 2 — Player class layout (Android libgof2hdaa.so, armv7 Thumb).
// Field offsets recovered from the per-method target disassembly.


struct Gun;
struct KIPlayer;
struct Vector;
struct Matrix;



struct Player {
    Array<Array<Gun *> *> *guns;     // 0x00  gun slots (array of arrays of guns)
    float transform[15];             // 0x04  AEMath::Matrix (this+4), 0x3c bytes -> ends 0x40
    int32_t radius;                  // 0x40
    uint16_t field_44;               // 0x44
    uint8_t pad_46[0x0e];            // 0x46
    uint16_t field_54;               // 0x54
    uint8_t pad_56[2];               // 0x56
    int32_t field_58;                // 0x58  (init -1)
    uint16_t enemyFlags;             // 0x5c  alwaysEnemy/alwaysFriend low/high byte pair
    uint8_t field_5e;                // 0x5e
    uint8_t pad_5f;                  // 0x5f
    int32_t field_60;                // 0x60
    int32_t field_64;                // 0x64  (0x67 byte = gamma-damage flag, set in damageGamma)
    uint16_t field_68;               // 0x68
    uint8_t pad_6a[2];               // 0x6a
    int32_t damageDoneByPlayer;      // 0x6c
    uint8_t playShootSound;          // 0x70
    uint8_t pad_71[3];               // 0x71
    Array<Player *> *enemies;        // 0x74
    int32_t hitpoints;               // 0x78
    int32_t empPoints;               // 0x7c
    int32_t maxEmpPoints;            // 0x80
    int32_t maxHitpoints;            // 0x84
    float shieldHP;                  // 0x88
    int32_t armorHP;                 // 0x8c
    int32_t maxArmorHP;              // 0x90
    int32_t maxShieldHP;             // 0x94
    int32_t damageRate;              // 0x98
    int32_t numPrimaryGuns;          // 0x9c
    int32_t numSecondaryGuns;        // 0xa0
    int32_t numTertiaryGuns;         // 0xa4
    int32_t shieldDamageRate;        // 0xa8
    int32_t armorDamageRate;         // 0xac
    int32_t empDamageRate;           // 0xb0
    int32_t field_b4;                // 0xb4
    float gammaHP;                   // 0xb8
    uint8_t pad_bc[4];               // 0xbc
    uint8_t active;                  // 0xc0
    uint8_t damaged;                 // 0xc1
    uint8_t vulnerable;              // 0xc2
    uint8_t shootingEnabled;         // 0xc3
    float hitVector[3];              // 0xc4 .. 0xcc
    KIPlayer *kiPlayer;              // 0xd0
    float bombForce;                 // 0xd4
    float empForce;                  // 0xd8
    uint8_t field_dc;                // 0xdc
    uint8_t pad_dd[3];               // 0xdd
    uint8_t turnedEnemy;             // 0xe0
    uint8_t pad_e1[3];               // 0xe1
    int32_t empData;                 // 0xe4
    uint8_t pad_e8[4];               // 0xe8
    uint8_t alwaysEnemy;             // 0xec
    uint8_t alwaysFriend;            // 0xed
    uint8_t neverAttack;             // 0xee
    uint8_t pad_ef;                  // 0xef
    int32_t engineEvent;             // 0xf0
    int32_t field_f4;                // 0xf4  (init -1)
    uint8_t field_f8;                // 0xf8
    uint8_t pad_f9[3];               // 0xf9
    float position[3];               // 0xfc .. 0x104
    uint8_t field_108;               // 0x108
    uint8_t pad_109[3];              // 0x109
    int32_t playShootSoundId;        // 0x10c
    int32_t field_110;               // 0x110
};

static_assert(sizeof(Array<Gun *>) == 0xc, "Array size");
static_assert(__builtin_offsetof(Player, radius) == 0x40, "radius");
static_assert(__builtin_offsetof(Player, enemies) == 0x74, "enemies");
static_assert(__builtin_offsetof(Player, hitpoints) == 0x78, "hitpoints");
static_assert(__builtin_offsetof(Player, shieldHP) == 0x88, "shieldHP");
static_assert(__builtin_offsetof(Player, damageRate) == 0x98, "damageRate");
static_assert(__builtin_offsetof(Player, gammaHP) == 0xb8, "gammaHP");
static_assert(__builtin_offsetof(Player, active) == 0xc0, "active");
static_assert(__builtin_offsetof(Player, shootingEnabled) == 0xc3, "shootingEnabled");
static_assert(__builtin_offsetof(Player, hitVector) == 0xc4, "hitVector");
static_assert(__builtin_offsetof(Player, kiPlayer) == 0xd0, "kiPlayer");
static_assert(__builtin_offsetof(Player, turnedEnemy) == 0xe0, "turnedEnemy");
static_assert(__builtin_offsetof(Player, alwaysEnemy) == 0xec, "alwaysEnemy");
static_assert(__builtin_offsetof(Player, neverAttack) == 0xee, "neverAttack");
static_assert(__builtin_offsetof(Player, engineEvent) == 0xf0, "engineEvent");
static_assert(__builtin_offsetof(Player, position) == 0xfc, "position");
static_assert(__builtin_offsetof(Player, playShootSoundId) == 0x10c, "playShootSoundId");
#endif
