#ifndef GOF2_GUN_H
#define GOF2_GUN_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "game/ItemSort.h"

// Galaxy on Fire 2 -- Gun (weapon / projectile emitter). Top-level, no namespace.
// Vector/Matrix are provided at global scope by common.h (math.h).

class Player;
class Sparks;
class Item;
class Level;

class Gun {
public:
    int lastHitKIPlayer;
    Player* owner;
    unsigned count;
    char* positions;                    // vertex buffer (byte-addressed Array<Vector>)
    int directionCount;
    char* velocities;                   // velocity buffer (byte-addressed Array<Vector>)
    int field_0x20;
    char* upVectors;
    int field_0x2c;
    char* hitPositions;                 // impact points (byte-addressed Array<Vector>)
    int level;
    int* lifetimes;                     // projectile time-left buffer (new int[count])
    uint8_t* hitFlags;
    int initialLifetime;
    int fireDelay;
    uint8_t active;
    uint8_t hitSmall;
    float field_0x50;
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
    Vector offset;                      // muzzle offset
    uint8_t ignited;
    char field_0x89;
    int32_t field_0x8c;
    int field_0x90;
    int field_0x94;
    int field_0x98;
    int fireIndex;
    int field_0xa4;
    uint8_t field_0xa8;
    uint8_t delayActive;
    char* wobbleOffsets;                // Array<Vector*> (byte-addressed header)
    float field_0xb0;
    Player* enemies;
    Sparks* impact;
    Player* target;
    Vector basePos;
    int field_0xcc;
    int field_0xd0;
    int field_0xd4;
    Vector targetDir;                   // direction toward target; .z reused as render translation
    Vector velocity;                    // cached muzzle velocity (dir * speed)
    uint8_t playerGun;
    int slotIndex;
    uint8_t levelCollision;
    uint8_t friendGun;
    float errorMagnitudePercentage;     // +0xfc
    int magnitude;                      // +0x100
    int customRadius;                   // +0x104
    uint8_t homing;                     // +0x108
    int* geometries;                    // +0x10c  per-projectile transform handles (new int[count])
    uint8_t* randomFlags;               // +0x110  per-projectile random flags (new uint8_t[count])

    Gun(int kind, int p2, unsigned count, int p4, int p5, int p6, float p7, Vector dir, Vector vel);
    ~Gun() noexcept(false);

    void calcCharacterCollision();
    void* getEnemies();
    int getMagnitude();
    void ignite();
    uint8_t isPlayerGun();
    void removeAllEnemies();
    void render();
    void setEnemy(Player* enemy);
    void setErrorMagnitudePercentage(int v);
    void setFriendGun(bool v);
    void setLevel(Level* lvl);
    void setImpact(Sparks* impact);
    void setIndex(int index);
    void setLevelCollision(bool v);
    void setMagnitude(int v);
    void setOffset(const Vector* v);
    void setOffset_ii(int a, int b);
    void setPlayerGun(bool v);
    void shoot(Matrix m, int n, bool b);
    void shootAt(Matrix m, int n, Player* p, bool b);
    void translate(const Vector* v);
    void update(int dt);
};
#endif
