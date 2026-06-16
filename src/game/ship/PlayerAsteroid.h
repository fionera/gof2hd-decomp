#ifndef GOF2_PLAYERASTEROID_H
#define GOF2_PLAYERASTEROID_H
#include "common.h"
#include "mathtypes.h"
class Hud;  // referenced only by pointer in KIPlayer's interface, below
#include "game/ship/KIPlayer.h"

class AEGeometry;
class Explosion;

// PlayerAsteroid: a minable / destructible asteroid driven by the AI controller.
// It extends KIPlayer with asteroid-specific state (loot quality, mining flags, a
// bomb-force push impulse and its own explosion VFX) while reusing the KIPlayer
// base for the Player wrapper, geometry node and teardown.
//
// The bomb-push impulse (timer/duration/direction/spin) and the dropped-loot list
// live in the KIPlayer base storage block; they are exposed here through the
// asteroid's own semantic names.
class PlayerAsteroid : public KIPlayer {
public:
    using Vector = AbyssEngine::AEMath::Vector;
    using Matrix = AbyssEngine::AEMath::Matrix;

    // Asteroid-specific members (beyond the KIPlayer base footprint).
    uint8_t    asteroidFlag;
    int        asteroidIndex;
    Explosion* explosion;
    int        lastDelta;
    float      scaling;
    uint8_t    minable;
    Vector     spin;            // per-axis idle rotation rate
    uint8_t    rotationEnabled;
    int        quality;
    int        lastHitpoints;
    int        hitFlashActive;
    float      hitFlashTimer;
    int        field_0x164;
    int        field_0x168;
    int        field_0x16c;

    PlayerAsteroid(int playerId, AEGeometry* geometry, int explosionType, int asteroidIndex,
                   const Vector& position, float scaling, int quality);
    ~PlayerAsteroid();

    void   setAsteroidIndex(int asteroidIndex);
    void   translate(const Vector& delta);
    void   render();
    void   setPosition(const Vector& position);
    void   outerCollide(Vector value);
    void   outerCollide(float x, float y, float z);
    Vector getPosition();
    void   setRotationEnabled(bool enabled);
    int    getQualityFrameIndex();
    int    getQuality();
    float  getScaling();
    uint8_t isMinable();
    String getQualityString();
    void   update(int delta);
    Vector getProjectionVector(const Vector& value);
    void   setAsteroidCenter(Vector center);
    bool   collide(float x, float y, float z);
    void   push(int delta);
    void   initPush(const Vector& target, int duration);

private:
    // Whether this asteroid should drop loot when destroyed (KIPlayer base slot 0x4c).
    int&  dropsLoot()        { return this->field_0x4c; }
    // Dropped-loot {itemId, count} list (reuses the KIPlayer base cargo slot 0x50).
    Array<int>*& loot()      { return this->cargo; }
    // Optional secondary mesh (reuses the KIPlayer base crateGeometry slot 0x78).
    AEGeometry*& secondaryGeometry() { return this->crateGeometry; }
    // Bomb-push impulse state, overlaid on the KIPlayer base block 0x104..0x120.
    int&    pushTimer()      { return this->field_0x104; }
    int&    pushDuration()   { return *reinterpret_cast<int*>(reinterpret_cast<char*>(&this->field_0x104) + 4); }
    Vector& pushDirection()  { return *reinterpret_cast<Vector*>(&this->field_0x10c); }
    Vector& pushSpin()       { return *reinterpret_cast<Vector*>(&this->field_0x118); }
};
#endif
