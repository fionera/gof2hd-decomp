#ifndef GOF2_PLAYER_H
#define GOF2_PLAYER_H
#include "gof2/common.h"
// real struct kept from byte-match recovery (+ supporting decls)
// Galaxy on Fire 2 — Player class layout (Android libgof2hdaa.so, armv7 Thumb).
// Field offsets recovered from the per-method target disassembly.


struct Gun;
struct KIPlayer;
// Vector and Matrix are AEMath 3D types brought in via common.h (using AEMath::Vector/Matrix).
// Do NOT forward-declare them here as plain structs — that conflicts with the using-declarations.



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
    void *engineEvent;               // 0xf0  FMOD event handle
    void *field_f4;                  // 0xf4  engine-sound position vec (init -1)
    uint8_t field_f8;                // 0xf8
    uint8_t pad_f9[3];               // 0xf9
    float position[3];               // 0xfc .. 0x104
    uint8_t field_108;               // 0x108
    uint8_t pad_109[3];              // 0x109
    int32_t playShootSoundId;        // 0x10c
    int32_t field_110;               // 0x110

    ~Player();                       // defined out-of-line in Player.cpp

    // ---- methods (converted from free functions) ----
    int GetEngineEvent();
    void calcWeaponSounds(int count);
    Player * ctor(int radius, int hitpoints, int numPrimary, int numSecondary, int numTertiary);
    void damage(int amount);
    void damageEmp(int amount, bool flag);
    void damageShip(int damage);
    unsigned char doesNeverAttack();
    int getArmorDamageRate();
    int getArmorHP();
    float getBombForce();
    int getCombinedHP();
    int getDamageRate();
    int getEmpDamageRate();
    float getEmpForce();
    int getEmpPoints();
    Player * getEnemy(int index);
    int getGammaHP();
    int getGunRegenRate();
    int getGunSlots();
    int getHitpoints();
    KIPlayer * getKIPlayer();
    int getMaxArmorHP();
    int getMaxEmpPoints();
    int getMaxHitpoints();
    int getMaxShieldHP();
    int getRadius();
    int getShieldDamageRate();
    int getShieldHP();
    unsigned char isActive();
    unsigned char isAlwaysEnemy();
    unsigned char isAlwaysFriend();
    unsigned char isDamaged();
    bool isDead();
    void regenerateArmor();
    void regenerateHull();
    void regenerateShield();
    void removeAllGuns();
    void resetDamageDoneByPlayer();
    void setActive(bool value);
    void setArmorHP(int value);
    void setBombForce(float value);
    void setEmpData(int points, int data);
    void setEmpForce(float value);
    float * setHitVector(float x, float y, float z);
    void setHitpoints(int value);
    void setKIPlayer(KIPlayer *value);
    void setMaxArmorHP(int value);
    void setMaxEmpPoints(int value);
    void setMaxHitpoints(int value);
    void setMaxShieldHP(int value);
    void setNeverAttack(bool value);
    void setPlayShootSound(bool play, int id);
    void setRadius(int value);
    void setShootingEnabled(bool value);
    void setVulnerable(bool value);
    void shoot(int a, int b, long long pos, bool flag);
    void shoot1(unsigned int slot, int idLo, int idHi, int flag, int m0, int m1, int m2, int m3, int m4, int m5, int m6, int m7, int m8, int m9, int m10, int m11, int m12, int m13, int m14);
    int shoot2(unsigned int slot, int gunId, int a4_00, int flag, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17, int a18, int a19, int a20, int a21, int a22);
    void turnEnemy();
    unsigned char turnedEnemy();
    Vector * update(int dt, int doSound);
    void updateDamageRate();
};

// NOTE: the original byte-matching decomp asserted the 32-bit ARM (ILP32) field offsets here
// (sizeof(Array<Gun*>)==0xc, radius==0x40, hitpoints==0x78, ...). Those invariants only hold on the
// 4-byte-pointer target; this is the native 64-bit macOS port (8-byte pointers, 24-byte std::vector),
// so the absolute offsets necessarily differ. Methods access fields by name, so the exact layout no
// longer needs to match — the target-only static_asserts are intentionally dropped for the 64-bit port.
#endif
