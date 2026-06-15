#ifndef GOF2_RADAR_H
#define GOF2_RADAR_H
#include "gof2/common.h"
#include "gof2/math.h"

// Cross-class types referenced only by pointer.
class Level;
class Hud;
class Player;
class Station;
class KIPlayer;

namespace AbyssEngine {

// Radar — the cockpit minimap/scanner: tracks the current lock target, projects
// world contacts into screen space and renders the radar overlay and lock label.
class Radar {
public:
    void* level;
    void* field_0x4;
    void* field_0x8;
    void* lockedAsteroid;
    int field_0x10;
    int field_0x14;
    int field_0x18;
    void* field_0x1c;
    void* lockedStation;
    Array<KIPlayer*>* players;
    void* field_0x38;
    int field_0x3c;
    int planetDockIndex;
    uint8_t enabled;
    int imageWidth;
    int imageHeight;
    uint8_t field_0x54;
    int field_0x58;
    int screenX;
    int screenY;
    volatile int centerX;
    volatile int centerY;
    volatile float weightX;
    volatile float weightY;
    int imageWidthSq;
    int imageHeightSq;
    int onScreen;
    uint8_t field_0x120;
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
    void* labelStrings;
    String lockLabel;
    uint8_t field_0x1a8;
    uint8_t scannerAvailable;
    int field_0x1b4;
    int field_0x1b8;
    int field_0x1bc;
    int radarImage;
    AEMath::Matrix transform;
    int field_0x20c;
    uint8_t drawMode;
    int screenWidth;
    int halfScreenWidth;
    int screenHeight;
    int halfScreenHeight;
    int originX;
    int originY;

    explicit Radar(Level* level);
    ~Radar();

    int getTurretScopeWidth();
    uint8_t hasScanner();
    uint8_t isPlasmaInRange();
    bool stationLocked();
    void unlockAsteroid();
    int getPlanetDockIndex();
    void update(KIPlayer* player);
    void update(AEMath::Vector value);
    long long draw(Player* player, Hud* hud, int mode);
    AEMath::Vector elipsoidIntersect(int y, int x, AEMath::Vector value);
    void drawCurrentLock(Hud* hud);
    void calcDistance(float p0, float a, float b, float c, float d, float e);
};

} // namespace AbyssEngine
#endif
