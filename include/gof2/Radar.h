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
    void* field_0x0;                    // +0x0
    void* field_0x4;                    // +0x4
    void* field_0x8;                    // +0x8
    void* field_0xc;                    // +0xc
    int field_0x10;                     // +0x10
    int field_0x14;                     // +0x14
    int field_0x18;                     // +0x18
    void* field_0x1c;                   // +0x1c
    void* field_0x24;                   // +0x24
    Array<KIPlayer *>* field_0x34;      // +0x34
    void* field_0x38;                   // +0x38
    int field_0x3c;                     // +0x3c
    int field_0x40;                     // +0x40
    uint8_t field_0x48;                 // +0x48
    int field_0x4c;                     // +0x4c
    int field_0x50;                     // +0x50
    uint8_t field_0x54;                 // +0x54
    int field_0x58;                     // +0x58
    int field_0xfc;                     // +0xfc
    int field_0x100;                    // +0x100
    volatile int field_0x104;           // +0x104
    volatile int field_0x108;           // +0x108
    volatile float field_0x10c;         // +0x10c
    volatile float field_0x110;         // +0x110
    int field_0x114;                    // +0x114
    int field_0x118;                    // +0x118
    int field_0x11c;                    // +0x11c
    uint8_t field_0x120;                // +0x120
    int field_0x12c;                    // +0x12c
    uint8_t field_0x130;                // +0x130
    int field_0x154;                    // +0x154
    float field_0x158;                  // +0x158
    float field_0x15c;                  // +0x15c
    int field_0x160;                    // +0x160
    int field_0x164;                    // +0x164
    int field_0x168;                    // +0x168
    int field_0x16c;                    // +0x16c
    int field_0x170;                    // +0x170
    int field_0x174;                    // +0x174
    int field_0x178;                    // +0x178
    int field_0x17c;                    // +0x17c
    int field_0x180;                    // +0x180
    void* field_0x188;                  // +0x188
    void* field_0x18c;                  // +0x18c
    uint8_t field_0x1a8;                // +0x1a8
    uint8_t field_0x1ab;                // +0x1ab
    int field_0x1b4;                    // +0x1b4
    int field_0x1b8;                    // +0x1b8
    int field_0x1bc;                    // +0x1bc
    int field_0x1c4;                    // +0x1c4
    int field_0x20c;                    // +0x20c
    uint8_t field_0x218;                // +0x218
    int field_0x21c;                    // +0x21c
    int field_0x220;                    // +0x220
    int field_0x224;                    // +0x224
    int field_0x228;                    // +0x228
    int field_0x22c;                    // +0x22c
    int field_0x230;                    // +0x230

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
