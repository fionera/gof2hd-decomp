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
    int field_0x58;

    uint8_t pad_0x5c_to_0x70[0x70 - 0x5c];
    uint8_t hasReservation;
    uint8_t pad_0x71_to_0x8c[0x8c - 0x71];
    uint8_t reservationDirty;
    uint8_t pad_0x8d_to_0xfc[0xfc - 0x8d]; // Ghidra: gap+field_0xa0/a4+gap+raceSprite/blipSprite/qualitySprite
    int screenX;
    int screenY;
    int centerX;
    int centerY;
    float weightX;
    float weightY;
    int imageWidthSq;
    int imageHeightSq;
    int onScreen;
    uint8_t field_0x120;
    int field_0x124; // Ghidra
    int field_0x128; // Ghidra
    int turretScopeHalfWidth;
    uint8_t plasmaInRange;
    int radarPosX;
    float radarPosY;
    float radarPosZ;
    // ASM (Radar ctor): labelStrings zeroed @0x188, lockLabel String ctor @0x18c, strb @0x1a8,
    // ints zeroed @0x1b4/0x1b8/0x1bc, transform ctor @0x1d0, @0x20c zeroed, screenWidth block
    // @0x21c-0x230 filled from the engine display size.
    uint8_t pad_0x140_to_0x160[0x160 - 0x140];
    int field_0x160;
    int field_0x164;
    int field_0x168;
    int field_0x16c;
    int field_0x170;
    int field_0x174;
    int field_0x178;
    int field_0x17c;
    int field_0x180;
    int field_0x184;
    Array<AbyssEngine::String *> *labelStrings;
    AbyssEngine::String lockLabel;
    uint8_t pad_0x198_to_0x1a8[0x1a8 - 0x198];
    uint8_t field_0x1a8;
    uint8_t field_0x1a9;
    uint8_t field_0x1aa;
    uint8_t scannerAvailable;
    int field_0x1ac;
    int field_0x1b0;
    int field_0x1b4;
    int field_0x1b8;
    int field_0x1bc;
    int radarImage;
    int field_0x1c4;
    int field_0x1c8;
    int field_0x1cc;
    AbyssEngine::AEMath::Matrix transform;
    int field_0x20c;
    uint8_t drawMode;
    int field_0x214;
    int field_0x218;
    int screenWidth;
    int halfScreenWidth;
    int screenHeight;
    int halfScreenHeight;
    int originX;
    int originY;

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

    long long draw(Player *player, Hud *hud, int mode);

    AbyssEngine::AEMath::Vector elipsoidIntersect(int y, int x, AbyssEngine::AEMath::Vector value);

    void drawCurrentLock(Hud *hud);

    void calcDistance(float p0, float a, float b, float c, float d, float e);

    static unsigned char drawTarget;
};

static_assert(offsetof(Radar, players) == 0x34, "Radar ctor stores players Array* @0x34");
static_assert(offsetof(Radar, planetDockIndex) == 0x40, "getPlanetDockIndex reads 0x40");
static_assert(offsetof(Radar, enabled) == 0x48, "ctor strb 1 @0x48");
static_assert(offsetof(Radar, hasReservation) == 0x70, "");
static_assert(offsetof(Radar, screenX) == 0xfc, "");
static_assert(offsetof(Radar, radarPosX) == 0x134, "ctor zeroes @0x134");
static_assert(offsetof(Radar, labelStrings) == 0x188, "ctor zeroes @0x188");
static_assert(offsetof(Radar, lockLabel) == 0x18c, "ctor String @0x18c");
static_assert(offsetof(Radar, scannerAvailable) == 0x1ab, "");
static_assert(offsetof(Radar, transform) == 0x1d0, "ctor Matrix @0x1d0");
static_assert(offsetof(Radar, screenWidth) == 0x21c, "ctor display block @0x21c-0x230");
static_assert(sizeof(Radar) == 0x234, "");

#endif
