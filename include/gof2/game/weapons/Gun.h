#ifndef GOF2_GUN_H
#define GOF2_GUN_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — Gun class (weapon/projectile emitter). Top-level, no namespace.
// Field offsets recovered from per-method target disassembly. We do NOT model a
// full layout: methods access fields via byte-offset casts from `this`.
//
// Known fields:
//   +0x08  uint  vertexCount
//   +0x0c  Vector* vertices (each 0xc bytes)
//   +0xb4  Player* enemies
//   +0xb8  Sparks* impact
//   +0xf0  bool  playerGun
//   +0xf8  bool  levelCollision
//   +0xf9  bool  friendGun
//   +0xfc  float errorMagnitudePercentage
//   +0x100 int   magnitude
//   +0x7c  Vector offset
//   +0xc0  Vector basePos     +0xd8 Vector targetDir     +0xe4 Vector velocity

struct Gun;

// AEGeometry is provided in full by gof2/AEGeometry.h (included by Gun.cpp). Only the +0xc
// geometry/transform handle is read here; forward-declare so Gun.h stays standalone.
struct AEGeometry;

class Gun {
public:
    int field_0x0;                      // +0x0
    Player* field_0x4;                  // +0x4
    unsigned count;                 // +0x8
    char* positions;                    // +0xc vertex buffer (byte-addressed)
    int field_0x14;                     // +0x14 len of 2nd embedded Array<Vector>
    char* velocities;                   // +0x18 velocity buffer (byte-addressed)
    int field_0x20;                     // +0x20 len of 3rd embedded Array<Vector>
    char* field_0x24;                   // +0x24 data ptr of 3rd embedded Array<Vector>
    int field_0x2c;                     // +0x2c len of 4th embedded Array<Vector>
    char* hitPositions;                 // +0x30 data ptr of 4th embedded Array<Vector> (impact points)
    int field_0x38;                     // +0x38
    void* lifetimes;                   // +0x3c (heap array)
    uint8_t* hitFlags;                // +0x40
    int initialLifetime;                     // +0x44
    int fireDelay;                     // +0x48
    uint8_t field_0x4c;                 // +0x4c
    uint8_t field_0x4d;                 // +0x4d
    float field_0x50;                   // +0x50
    uint8_t field_0x54;                 // +0x54
    int itemIndex;                     // +0x58
    int weaponType;                     // +0x5c
    int field_0x60;                     // +0x60
    int field_0x64;                     // +0x64
    uint8_t field_0x68;                 // +0x68
    int timer;                     // +0x6c
    int delayTimer;                     // +0x70
    int field_0x74;                     // +0x74
    int field_0x78;                     // +0x78
    Vector offset;                      // +0x7c muzzle offset (x,y,z)
    uint8_t field_0x88;                 // +0x88
    char field_0x89;                    // +0x89
    int32_t field_0x8c;                 // +0x8c
    int field_0x90;                     // +0x90
    int field_0x94;                     // +0x94
    int field_0x98;                     // +0x98
    int field_0xa0;                     // +0xa0
    int field_0xa4;                     // +0xa4
    char field_0xa5;                    // +0xa5
    uint8_t field_0xa6;                 // +0xa6
    uint8_t field_0xa7;                 // +0xa7
    uint8_t field_0xa8;                 // +0xa8
    uint8_t delayActive;                 // +0xa9
    char* field_0xac;                   // +0xac Array<Vector*> (byte-addressed header)
    float field_0xb0;                   // +0xb0
    Player* enemies;                 // +0xb4 enemies
    Sparks* impact;                 // +0xb8 impact
    Player* target;                 // +0xbc
    Vector basePos;                     // +0xc0 enemy/base position (x,y,z)
    int field_0xcc;                     // +0xcc
    int field_0xd0;                     // +0xd0
    int field_0xd4;                     // +0xd4
    Vector targetDir;                   // +0xd8 direction toward target (x,y,z); .z reused as render translation
    Vector velocity;                    // +0xe4 cached muzzle velocity (dir * speed)
    uint8_t playerGun;                 // +0xf0
    int field_0xf4;                     // +0xf4
    uint8_t levelCollision;                 // +0xf8
    uint8_t friendGun;                 // +0xf9
    float errorMagnitudePercentage;                   // +0xfc
    int magnitude;                    // +0x100
    uint8_t field_0x108;                // +0x108
    void* geometries;                  // +0x10c (heap array)
    void* randomFlags;                  // +0x110 (heap array)

    ~Gun() noexcept(false);

    // ---- methods (converted from free functions) ----
    void calcCharacterCollision();
    Gun * ctor(int kind, int p2, unsigned count, int p4, int p5, int p6, float p7, Vector dir, Vector vel);
    void * getEnemies();
    int getMagnitude();
    void ignite();
    uint8_t isPlayerGun();
    void removeAllEnemies();
    void render();
    void setEnemy(Player *enemy);
    void setErrorMagnitudePercentage(int v);
    void setFriendGun(bool v);
    void setImpact(Sparks *impact);
    void setIndex(int index);
    void setLevelCollision(bool v);
    void setMagnitude(int v);
    void setOffset(const Vector *v);
    void setOffset_ii(int a, int b);
    void setPlayerGun(bool v);
    void shoot(Matrix m, int n, bool b);
    void shootAt(Matrix m, int n, Player *p, bool b);
    void translate(const Vector *v);
    void update(int dt);
};
#endif
