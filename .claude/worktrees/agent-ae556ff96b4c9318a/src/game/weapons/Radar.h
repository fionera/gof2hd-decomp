#ifndef GOF2_RADAR_H
#define GOF2_RADAR_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "mathtypes.h"

// Cross-class types referenced only by pointer.
class Level;
class Hud;
class Player;
class Station;
class KIPlayer;

// Radar — the cockpit minimap/scanner: tracks the current lock target, projects
// world contacts into screen space and renders the radar overlay and lock label.
// Top-level class (mangles as _ZN5Radar...); the AbyssEngine math/string types it
// holds are referenced fully qualified.
class Radar {
public:
    void* level;                       // +0x00
    KIPlayer* lockedEnemy;             // +0x04
    void* field_0x8;                   // +0x08
    KIPlayer* lockedAsteroid;          // +0x0c
    int field_0x10;                    // +0x10
    int field_0x14;                    // +0x14
    int field_0x18;                    // +0x18
    void* field_0x1c;                  // +0x1c
    void* lockedStation;               // +0x24
    Array<KIPlayer*>* players;         // +0x34
    KIPlayer* lockedGasCloud;          // +0x38
    int field_0x3c;                    // +0x3c
    int planetDockIndex;               // +0x40
    uint8_t enabled;                   // +0x48
    int imageWidth;                    // +0x4c
    int imageHeight;                   // +0x50
    uint8_t field_0x54;                // +0x54
    int field_0x58;                    // +0x58
    int screenX;                       // +0xfc
    int screenY;                       // +0x100
    volatile int centerX;              // +0x104
    volatile int centerY;              // +0x108
    volatile float weightX;            // +0x10c
    volatile float weightY;            // +0x110
    int imageWidthSq;                  // +0x114
    int imageHeightSq;                 // +0x118
    int onScreen;                      // +0x11c
    uint8_t field_0x120;               // +0x120
    int turretScopeHalfWidth;          // +0x12c
    uint8_t plasmaInRange;             // +0x130
    int radarPosX;                     // +0x154
    float radarPosY;                   // +0x158
    float radarPosZ;                   // +0x15c
    int field_0x160;                   // +0x160
    int field_0x164;                   // +0x164
    int field_0x168;                   // +0x168
    int field_0x16c;                   // +0x16c
    int field_0x170;                   // +0x170
    int field_0x174;                   // +0x174
    int field_0x178;                   // +0x178
    int field_0x17c;                   // +0x17c
    int field_0x180;                   // +0x180
    Array<AbyssEngine::String*>* labelStrings; // +0x188
    AbyssEngine::String lockLabel;     // +0x18c
    uint8_t field_0x1a8;               // +0x1a8
    uint8_t scannerAvailable;          // +0x1ab
    int field_0x1b4;                   // +0x1b4
    int field_0x1b8;                   // +0x1b8 (scanRange)
    int field_0x1bc;                   // +0x1bc
    int radarImage;                    // +0x1c4
    AbyssEngine::AEMath::Matrix transform; // +0x1c8
    int field_0x20c;                   // +0x20c
    uint8_t drawMode;                  // +0x218
    int screenWidth;                   // +0x21c
    int halfScreenWidth;               // +0x220
    int screenHeight;                  // +0x224
    int halfScreenHeight;              // +0x228
    int originX;                       // +0x22c
    int originY;                       // +0x230

    explicit Radar(Level* level);
    ~Radar();

    int getTurretScopeWidth();
    uint8_t hasScanner();
    uint8_t isPlasmaInRange();
    bool stationLocked();
    KIPlayer* getLockedEnemy();
    KIPlayer* getLockedAsteroid();
    KIPlayer* getLockedGasCloud();
    void unlockAsteroid();
    int getPlanetDockIndex();
    void update(KIPlayer* player);
    void update(AbyssEngine::AEMath::Vector value);
    long long draw(Player* player, Hud* hud, int mode);
    AbyssEngine::AEMath::Vector elipsoidIntersect(int y, int x, AbyssEngine::AEMath::Vector value);
    void drawCurrentLock(Hud* hud);
    void calcDistance(float p0, float a, float b, float c, float d, float e);
};

#endif
