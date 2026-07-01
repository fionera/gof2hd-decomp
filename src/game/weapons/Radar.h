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
    Array<KIPlayer *> *players;
    KIPlayer *lockedGasCloud;
    int field_0x3c;
    int planetDockIndex;
    uint8_t enabled;
    int imageWidth;
    int imageHeight;
    uint8_t field_0x54;
    int field_0x58;

    uint8_t pad_0x48_to_0x70[0x70 - 0x4c]; // shrunk 4 to absorb field_0x20 above; hasReservation stays @0x70
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
    int field_0x160;
    int field_0x164;
    int field_0x168;
    int field_0x16c;
    int field_0x170;
    int field_0x174;
    int field_0x178;
    int field_0x17c;
    int field_0x180;
    Array<AbyssEngine::String *> *labelStrings;
    AbyssEngine::String lockLabel;
    uint8_t field_0x1a8;
    uint8_t scannerAvailable;
    int field_0x1b4;
    int field_0x1b8;
    int field_0x1bc;
    int radarImage;
    AbyssEngine::AEMath::Matrix transform;
    int field_0x20c;
    uint8_t drawMode;
    int screenWidth;
    int halfScreenWidth;
    int screenHeight;
    int halfScreenHeight;
    int originX;
    int originY;

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

#endif
