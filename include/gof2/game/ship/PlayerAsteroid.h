#ifndef GOF2_PLAYERASTEROID_H
#define GOF2_PLAYERASTEROID_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {

namespace AEMath {

} // namespace AEMath

} // namespace AbyssEngine

using String = AbyssEngine::String;
using Vector = AbyssEngine::AEMath::Vector;
using Matrix = AbyssEngine::AEMath::Matrix;

struct AEGeometry;
struct Player;
struct Explosion;
struct TargetFollowCamera;
struct ArrayInt;

extern "C" void *operator_new(uint32_t size);
extern "C" void operator_delete(void *ptr);

class PlayerAsteroid {
public:
    void* vtable;                    // +0x0
    Player* player;                  // +0x4
    AEGeometry* geometry;              // +0x8
    uint8_t field_0x3c;                 // +0x3c
    uint8_t dropsLoot;                 // +0x4c
    ArrayInt* loot;               // +0x50
    AEGeometry* secondaryGeometry;             // +0x78
    int state;                     // +0x88
    uint8_t visible;                 // +0xf5
    int pushTimer;                    // +0x104
    int pushDuration;                    // +0x108
    float pushSpinX;                  // +0x118
    float pushSpinY;                  // +0x11c
    float pushSpinZ;                  // +0x120
    uint8_t field_0x124;                // +0x124
    int asteroidIndex;                    // +0x128
    Explosion* explosion;             // +0x12c
    int lastDelta;                    // +0x134
    float scaling;                  // +0x138
    uint8_t minable;                // +0x13c
    int spinX;                    // +0x140
    int spinY;                    // +0x144
    int spinZ;                    // +0x148
    uint8_t rotationEnabled;                // +0x14c
    int quality;                    // +0x150
    int lastHitpoints;                    // +0x158
    int hitFlashActive;                    // +0x15c
    float hitFlashTimer;                  // +0x160
    int field_0x164;                    // +0x164
    int field_0x168;                    // +0x168
    int field_0x16c;                    // +0x16c

    PlayerAsteroid(int playerId, AEGeometry *geometry, int explosionType, int asteroidIndex,
                   const Vector &position, float scaling, int quality);
    ~PlayerAsteroid();
    void setAsteroidIndex(int asteroidIndex);
    void translate(const Vector &delta);
    void render();
    void setPosition(const Vector &position);
    void outerCollide(Vector value);
    void outerCollide(float x, float y, float z);
    Vector getPosition();
    void setRotationEnabled(bool enabled);
    int getQualityFrameIndex();
    int getQuality();
    float getScaling();
    uint8_t isMinable();
    String getQualityString();
    void update(int delta);
    Vector getProjectionVector(const Vector &value);
    void setAsteroidCenter(Vector center);
    bool collide(float x, float y, float z);
    void push(int delta);
    void initPush(const Vector &target, int duration);
    // Complete-object destructor helper (the D0 deleting destructor's first half):
    // run ~PlayerAsteroid() and hand back `this` so the caller can free the storage.
    void *completeDtor();
};
#endif
