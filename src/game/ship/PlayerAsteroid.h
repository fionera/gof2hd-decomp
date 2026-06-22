#ifndef GOF2_PLAYERASTEROID_H
#define GOF2_PLAYERASTEROID_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/math/Matrix.h"
class Hud; // referenced only by pointer in KIPlayer's interface, below
#include "game/ship/KIPlayer.h"

class AEGeometry;
class Explosion;

class PlayerAsteroid : public KIPlayer {
public:
    using Vector = AbyssEngine::AEMath::Vector;
    using Matrix = AbyssEngine::AEMath::Matrix;

    // Asteroid-specific members (beyond the KIPlayer base footprint).
    uint8_t asteroidFlag;
    int asteroidIndex;
    Explosion *explosion;
    int lastDelta;
    float scaling;
    uint8_t minable;
    Vector spin; // per-axis idle rotation rate
    uint8_t rotationEnabled;
    int quality;
    int lastHitpoints;
    int hitFlashActive;
    float hitFlashTimer;
    int field_0x164;
    int field_0x168;
    int field_0x16c;

    PlayerAsteroid(int playerId, AEGeometry *geometry, int explosionType, int asteroidIndex,
                   const Vector &position, float scaling, int quality);

    ~PlayerAsteroid();

    void setAsteroidIndex(int asteroidIndex);

    void translate(const Vector &delta) override; // actor vtable slot +0x20
    void render() override;

    void setPosition(const Vector &position);

    int outerCollide(float x, float y, float z) override; // slot +0x3c -> this->collide
    int outerCollide(Vector point);

    Vector getPosition();

    void setRotationEnabled(bool enabled);

    int getQualityFrameIndex();

    int getQuality();

    float getScaling();

    uint8_t isMinable();

    String getQualityString();

    void update(int delta) override;

    Vector getProjectionVector(const Vector &value) override; // actor vtable slot +0x50
    void setAsteroidCenter(Vector center);

    int collide(float x, float y, float z) override; // actor vtable slot +0x38
    void push(int delta) override;

    void initPush(const Vector &target, int duration) override; // actor vtable slot +0x2c

private:
    // Whether this asteroid should drop loot when destroyed (KIPlayer base slot 0x4c).
    int &dropsLoot() { return this->hasCargo; }
    // Dropped-loot {itemId, count} list (reuses the KIPlayer base cargo slot 0x50).
    Array<int> *&loot() { return this->cargo; }
    // Optional secondary mesh (reuses the KIPlayer base crateGeometry slot 0x78).
    AEGeometry *&secondaryGeometry() { return this->crateGeometry; }
    // Bomb-push impulse state, overlaid on the KIPlayer base block 0x104..0x120.
    int &pushTimer() { return this->field_0x104; }
    int &pushDuration() { return *reinterpret_cast<int *>(reinterpret_cast<char *>(&this->field_0x104) + 4); }
    Vector &pushDirection() { return *reinterpret_cast<Vector *>(&this->field_0x10c); }
    Vector &pushSpin() { return *reinterpret_cast<Vector *>(&this->field_0x118); }
};
#endif
