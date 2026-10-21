#ifndef GOF2_RADAR_H
#define GOF2_RADAR_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "engine/math/Matrix.h"
#include "game/ship/KIPlayer.h"

#include "engine/math/AEMath.h"
#include "engine/math/Vector.h"

class Player;

class Hud;
class KIPlayer;
class Level;
class Route;
class Sprite;

class Radar {
public:
    Level *level;

    KIPlayer *lockedEnemy;

    KIPlayer *dockNavPtr;

    KIPlayer *lockedAsteroid;
    int field_0x10;
    int field_0x14;
    int field_0x18;
    KIPlayer *field_0x1c;
    // ASM: stationLocked reads lockedStation at [this,#36]=0x24; ours had it @0x20. 4-byte field precedes.
    int field_0x20;
    KIPlayer *lockedStation;
    // ASM (Radar ctor @0x12d484): players Array* stored @0x34, planetDockIndex zeroed @0x40,
    // enabled=1 strb @0x48, imageWidth/imageHeight strd @0x4c/0x50, strb 0 @0x54, @0x58.
    int field_0x28;
    int field_0x2c;
    int field_0x30;
    Array<KIPlayer *> *players;
    KIPlayer *lockedGasCloud;
    int field_0x3c;
    int planetDockIndex;
    int field_0x44;
    uint8_t enabled;
    int imageWidth;
    int imageHeight;
    uint8_t field_0x54;
    int *asteroidSlots; // 0x58: 5 candidate asteroid indices (-1 = empty)

    unsigned int image_0x5c;
    unsigned int image_0x60;
    unsigned int image_0x64;
    unsigned int image_0x68;
    unsigned int image_0x6c;
    // 0x70 / 0x8c are image handles in the ctor; PlayerEgo reads their low byte as flags.
    uint8_t hasReservation;
    uint8_t pad_0x71[3];
    unsigned int image_0x74;
    unsigned int image_0x78;
    unsigned int image_0x7c;
    unsigned int image_0x80;
    unsigned int image_0x84;
    unsigned int image_0x88;
    uint8_t reservationDirty;
    uint8_t pad_0x8d[3];
    unsigned int image_0x90;
    unsigned int image_0x94;
    unsigned int image_0x98;
    unsigned int image_0x9c;
    unsigned int image_0xa0;
    unsigned int image_0xa4;
    unsigned int image_0xa8;
    unsigned int image_0xac;
    unsigned int image_0xb0;
    unsigned int image_0xb4;
    unsigned int image_0xb8;
    unsigned int image_0xbc;
    unsigned int image_0xc0;
    unsigned int image_0xc4;
    unsigned int image_0xc8;
    unsigned int image_0xcc;
    unsigned int image_0xd0;
    unsigned int image_0xd4;
    unsigned int image_0xd8;
    unsigned int image_0xdc;
    unsigned int image_0xe0;
    unsigned int image_0xe4;
    unsigned int image_0xe8;
    unsigned int image_0xec;
    Sprite *lockSprite;
    Sprite *blipSprite;
    Sprite *qualitySprite;
    int screenX;
    int screenY;
    int centerX;
    int centerY;
    float weightX;
    float weightY;
    int imageWidthSq;
    int imageHeightSq;
    uint8_t onScreen;
    uint8_t field_0x11d;
    uint8_t field_0x11e;
    uint8_t field_0x11f;
    uint8_t field_0x120;
    int field_0x124; // Ghidra
    int field_0x128; // Ghidra
    int turretScopeHalfWidth;
    uint8_t plasmaInRange;
    Array<KIPlayer *> *enemies;       // 0x134
    Array<KIPlayer *> *landmarks;     // 0x138
    Array<KIPlayer *> *asteroids;     // 0x13c
    Array<KIPlayer *> *planetTargets; // 0x140
    Array<KIPlayer *> *gasClouds;     // 0x144
    int gasLockTimer;                 // 0x148
    int field_0x14c;
    Route *route;                     // 0x150
    // radar-space position of the tracked target (Vector), written by update()
    float radarPosX;
    float radarPosY;
    float radarPosZ;
    // ASM (Radar ctor): labelStrings zeroed @0x188, lockLabel String ctor @0x18c, strb @0x1a8,
    // ints zeroed @0x1b4/0x1b8/0x1bc, transform ctor @0x1d0, @0x20c zeroed, screenWidth block
    // @0x21c-0x230 filled from the engine display size.
    AbyssEngine::AEMath::Vector playerPos;  // 0x160
    AbyssEngine::AEMath::Vector egoPos;     // 0x16c
    AbyssEngine::AEMath::Vector targetPos;  // 0x178
    int barWidth;                           // 0x184
    Array<AbyssEngine::String *> *labelStrings;
    AbyssEngine::String lockLabel;
    int enemyLockTimer;    // 0x198
    int asteroidLockTimer; // 0x19c
    int planetLockTimer;   // 0x1a0
    int stationLockTimer;  // 0x1a4
    uint8_t field_0x1a8;
    uint8_t field_0x1a9;
    uint8_t field_0x1aa;
    uint8_t scannerAvailable;
    uint8_t hasScanner19;
    uint8_t hasTractor13;
    uint8_t tractorAuto;
    uint8_t tractorMode2;
    uint8_t field_0x1b0;
    uint8_t pad_0x1b1[3];
    int field_0x1b4;
    int field_0x1b8;
    int field_0x1bc;
    int radarImage;
    unsigned int field_0x1c4; // radar background image
    int field_0x1c8;
    int field_0x1cc;
    AbyssEngine::AEMath::Matrix transform;
    int field_0x20c;
    int lockLabelX; // centred x of the lock label background
    int field_0x214;
    int field_0x218;
    int screenWidth;
    int halfScreenWidth;
    int screenHeight;
    int halfScreenHeight;
    int originX;
    int originY;
    int field_0x234;
    int field_0x238;
    AbyssEngine::AEMath::Vector cameraPos; // 0x23c

    static_assert(sizeof(AbyssEngine::String) == 0xc, "lockLabel must be 12 bytes");

    explicit Radar(Level *level);

    ~Radar();

    int getTurretScopeWidth();

    uint8_t hasScanner();

    uint8_t isPlasmaInRange();

    bool stationLocked();

    KIPlayer *getLockedEnemy();

    KIPlayer *getLockedAsteroid();

    KIPlayer *getLockedGasCloud();

    void unlockAsteroid();

    int getPlanetDockIndex();

    void update(KIPlayer *player);

    void update(AbyssEngine::AEMath::Vector value);

    void draw(Player *player, Hud *hud, int mode);

    AbyssEngine::AEMath::Vector elipsoidIntersect(int y, int x, AbyssEngine::AEMath::Vector value);

    void drawCurrentLock(Hud *hud);

    AbyssEngine::String calcDistance(float x1, float y1, float z1, float x2, float y2, float z2);

    static unsigned char drawTarget;
};

static_assert(offsetof(Radar, players) == 0x34, "Radar ctor stores players Array* @0x34");
static_assert(offsetof(Radar, planetDockIndex) == 0x40, "getPlanetDockIndex reads 0x40");
static_assert(offsetof(Radar, enabled) == 0x48, "ctor strb 1 @0x48");
static_assert(offsetof(Radar, hasReservation) == 0x70, "");
static_assert(offsetof(Radar, screenX) == 0xfc, "");
static_assert(offsetof(Radar, radarPosX) == 0x154, "update() writes the radar-space Vector @0x154");
static_assert(offsetof(Radar, labelStrings) == 0x188, "ctor zeroes @0x188");
static_assert(offsetof(Radar, lockLabel) == 0x18c, "ctor String @0x18c");
static_assert(offsetof(Radar, scannerAvailable) == 0x1ab, "");
static_assert(offsetof(Radar, transform) == 0x1d0, "ctor Matrix @0x1d0");
static_assert(offsetof(Radar, screenWidth) == 0x21c, "ctor display block @0x21c-0x230");
static_assert(offsetof(Radar, cameraPos) == 0x23c, "");
static_assert(sizeof(Radar) == 0x248, "MGame news Radar with 584 bytes");

#endif
