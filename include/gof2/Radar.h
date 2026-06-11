#ifndef GOF2_RADAR_H
#define GOF2_RADAR_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct Level;
struct Hud;
struct Player;
struct Station;

namespace AbyssEngine {


namespace AEMath {
struct Matrix;



Vector MatrixTransformVector(Matrix const &, Vector const &);
}

struct PaintCanvas {
    int GetScreenPosition(AEMath::Vector const &, AEMath::Vector &);
};
}



struct KIPlayer {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual AbyssEngine::AEMath::Vector getPosition();
};

struct SolarSystem {
    Array<Station *> *getStations();
};

struct Status {
    SolarSystem *getSystem();
};

struct Globals {
    float sqrt(float);
};

extern Status *gStatus;
extern Globals *gGlobals;
extern AbyssEngine::PaintCanvas *gPaintCanvas;

void operator delete(void *) noexcept;

struct Radar {
    void* level;                    // +0x0
    void* field_0x4;                    // +0x4
    void* field_0x8;                    // +0x8
    void* lockedAsteroid;                    // +0xc
    int field_0x10;                     // +0x10
    int field_0x14;                     // +0x14
    int field_0x18;                     // +0x18
    void* field_0x1c;                   // +0x1c
    void* lockedStation;                   // +0x24
    Array<KIPlayer *>* players;      // +0x34
    void* field_0x38;                   // +0x38
    int field_0x3c;                     // +0x3c
    int planetDockIndex;                     // +0x40
    uint8_t enabled;                 // +0x48
    int imageWidth;                     // +0x4c
    int imageHeight;                     // +0x50
    uint8_t field_0x54;                 // +0x54
    int field_0x58;                     // +0x58
    int screenX;                     // +0xfc
    int screenY;                    // +0x100
    volatile int centerX;           // +0x104
    volatile int centerY;           // +0x108
    volatile float weightX;         // +0x10c
    volatile float weightY;         // +0x110
    int imageWidthSq;                    // +0x114
    int imageHeightSq;                    // +0x118
    int onScreen;                    // +0x11c
    uint8_t field_0x120;                // +0x120
    int turretScopeHalfWidth;                    // +0x12c
    uint8_t plasmaInRange;                // +0x130
    int radarPosX;                    // +0x154
    float radarPosY;                  // +0x158
    float radarPosZ;                  // +0x15c
    int field_0x160;                    // +0x160
    int field_0x164;                    // +0x164
    int field_0x168;                    // +0x168
    int field_0x16c;                    // +0x16c
    int field_0x170;                    // +0x170
    int field_0x174;                    // +0x174
    int field_0x178;                    // +0x178
    int field_0x17c;                    // +0x17c
    int field_0x180;                    // +0x180
    void* labelStrings;                  // +0x188
    void* lockLabel;                  // +0x18c
    uint8_t field_0x1a8;                // +0x1a8
    uint8_t scannerAvailable;                // +0x1ab
    int field_0x1b4;                    // +0x1b4
    int field_0x1b8;                    // +0x1b8
    int field_0x1bc;                    // +0x1bc
    int radarImage;                    // +0x1c4
    int field_0x20c;                    // +0x20c
    uint8_t drawMode;                // +0x218
    int screenWidth;                    // +0x21c
    int halfScreenWidth;                    // +0x220
    int screenHeight;                    // +0x224
    int halfScreenHeight;                    // +0x228
    int originX;                    // +0x22c
    int originY;                    // +0x230

    Radar(Level *level);
    ~Radar();
    int getTurretScopeWidth();
    uint8_t hasScanner();
    uint8_t isPlasmaInRange();
    bool stationLocked();
    void unlockAsteroid();
    int getPlanetDockIndex();
    void update(KIPlayer *player);
    void update(AbyssEngine::AEMath::Vector value);
    long long draw(Player *player, Hud *hud, int mode);
    AbyssEngine::AEMath::Vector elipsoidIntersect(int y, int x, AbyssEngine::AEMath::Vector value);
    void drawCurrentLock(Hud *hud);
    void calcDistance(float p0, float a, float b, float c, float d, float e);
};
#endif
