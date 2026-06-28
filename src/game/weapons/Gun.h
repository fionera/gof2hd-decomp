#ifndef GOF2_GUN_H
#define GOF2_GUN_H
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
    int field_0x20;
    char *upVectors;
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

    union {
        // lint: union_decl int field_0xa4 (whole-slot, set in Gun.cpp/Level.cpp/ObjectGun.cpp) overlaps individual bytes field_0xa5/0xa6/0xa7 (read in ObjectGun.cpp) — genuine cross-file type-pun, layout-fixed
        int field_0xa4;

        struct {
            uint8_t field_0xa4_b0;
            uint8_t field_0xa5;
            uint8_t field_0xa6;
            uint8_t field_0xa7;
        };
    };

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
#endif
