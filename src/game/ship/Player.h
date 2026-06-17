#ifndef GOF2_PLAYER_H
#define GOF2_PLAYER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// Galaxy on Fire 2 -- Player: a controllable/AI ship instance (hull, shields,
// armor, EMP, guns and the engine-sound state). Top-level, no namespace.
// Vector/Matrix are the AEMath 3D types brought in via common.h.

class Gun;
class KIPlayer;

class Player {
public:
    Array<Array<Gun *> *> *guns;     // +0x00 gun slots (array of arrays of guns)
    float transform[15];             // +0x04 AEMath::Matrix (3x4 row-major)
    int32_t radius;                  // +0x40
    uint16_t destroyed;              // +0x44 killed-by-player flag
    uint8_t pad_46[0x0e];
    uint16_t field_54;               // +0x54
    uint8_t pad_56[2];
    int32_t field_58;                // +0x58
    uint16_t enemyFlags;             // +0x5c alwaysEnemy/alwaysFriend low/high byte pair
    uint8_t field_5e;                // +0x5e
    uint8_t pad_5f;
    float flShake;                   // +0x60 hit/damage shake accumulator
    uint8_t shieldHit;               // +0x64 shield-hit flag this frame
    uint8_t armorHit;                // +0x65 armor-hit flag this frame
    uint8_t hullHit;                 // +0x66 hull-hit flag this frame
    uint8_t gammaHit;                // +0x67 gamma-hit flag this frame
    uint16_t empDisabled;            // +0x68 EMP-stun (empPoints recharging) flag
    uint8_t pad_6a[2];
    int32_t damageDoneByPlayer;      // +0x6c
    uint8_t playShootSound;          // +0x70
    uint8_t pad_71[3];
    Array<Player *> *enemies;
    int32_t hitpoints;
    int32_t empPoints;
    int32_t maxEmpPoints;
    int32_t maxHitpoints;
    float shieldHP;
    int32_t armorHP;
    int32_t maxArmorHP;
    int32_t maxShieldHP;
    int32_t damageRate;
    int32_t numPrimaryGuns;
    int32_t numSecondaryGuns;
    int32_t numTertiaryGuns;
    int32_t shieldDamageRate;
    int32_t armorDamageRate;
    int32_t empDamageRate;
    int32_t damageTimer;             // +0xb4 ms since last damage (clears 'damaged')
    float gammaHP;
    uint8_t pad_bc[4];
    uint8_t active;
    uint8_t damaged;
    uint8_t vulnerable;
    uint8_t shootingEnabled;
    float hitVector[3];
    KIPlayer *kiPlayer;
    float bombForce;
    float empForce;
    uint8_t field_dc;
    uint8_t pad_dd[3];
    uint8_t turnedEnemy;
    uint8_t pad_e1[3];
    int32_t empData;
    uint8_t pad_e8[4];
    uint8_t alwaysEnemy;
    uint8_t alwaysFriend;
    uint8_t neverAttack;
    uint8_t pad_ef;
    void *engineEvent;               // +0xf0 FMOD event handle
    void *enginePositionVec;         // +0xf4 engine-sound position vector
    uint8_t enginePaused;            // +0xf8 engine-sound paused flag
    uint8_t pad_f9[3];
    float position[3];               // +0xfc cached engine-sound emitter position
    uint8_t engineSoundPlaying;      // +0x108 engine-sound active flag
    uint8_t pad_109[3];
    int32_t playShootSoundId;        // +0x10c
    float healAccumulator;           // +0x110 fractional hull-regen accumulator

    Player(int radius, int hitpoints, int numPrimary, int numSecondary, int numTertiary);
    ~Player();

    int GetEngineEvent();
    void calcWeaponSounds(int count);
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
    void regenerateShield(float amount);
    void removeAllGuns();
    void resetDamageDoneByPlayer();
    void setActive(bool value);
    void awake(bool active);
    void setDead();
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
    Vector * update(int dt, bool doSound);
    void updateDamageRate();

    void pitchAllPrimaryGuns(float pitch);
    void damageHull(int damage);
    void damageShield(int damage);
    float damageGamma(float amount);
    Array<Player *> *getEnemies();
    void getPosition(Vector *out);
    void getHitVector(Vector *out);
    int replaceGuns(int a, int b, int c, int d, int e, bool f);
    bool isAsteroid();
    bool isGasCloud();
    bool gunAvailable(int slot);
    void heal(float amount);
    void setShieldHP(int value);
    void setGammaHP(int value);
    void refillGunDelay(int slot);
    void resetGunDelay(int slot);
    void setAlwaysEnemy(bool value);
    void setAlwaysFriend(bool value);
    void reset();
    void setEnemies(Array<Player *> *enemies);
    void addEnemies(Array<Player *> *enemies);
    void setEnemy(Player *enemy);
    void addEnemy(Player *enemy);
    void addGun(Array<Gun *> *gunsIn, int slot);
    void addGun(Gun *gun, int slot);
    void stopShooting(int slot);
    void stopShooting(int slot, int channel);
    void stopShootSound(int index, int channel);
    void PlayEngineSound(Vector *vec);
    void PauseEngineSound();
    void ResumeEngineSound(bool force);
    void StopEngineSound();
};

// The original exposes a 'turnedEnemy' accessor, but C++ forbids a member function
// sharing the name of the 'turnedEnemy' data member, so it lives here as a free helper.
inline unsigned char Player_turnedEnemy(Player *self) { return self->turnedEnemy; }

#endif
