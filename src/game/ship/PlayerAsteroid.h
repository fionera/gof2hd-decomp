#ifndef GOF2_PLAYERASTEROID_H
#define GOF2_PLAYERASTEROID_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "engine/math/Matrix.h"
#include "game/mission/Explosion.h"

#include "game/ship/KIPlayer.h"

#include "engine/math/Vector.h"
class AEGeometry;
class Explosion;

class PlayerAsteroid : public KIPlayer {
public:
    uint8_t asteroidFlag;
    int asteroidIndex;
    Explosion *explosion;
    // Ghidra ground truth: a 4-byte field sits at 0x130 between explosion@0x12c and lastDelta@0x134;
    // our decomp lacked it, shifting lastDelta/scaling/spin/.. 4 bytes low.
    int field_0x130;
    int lastDelta;
    float scaling;
    uint8_t minable;
    Vector spin;
    uint8_t rotationEnabled;
    int quality;
    int lastHitpoints;
    int hitFlashActive;
    float hitFlashTimer;
    // Ghidra ground truth: float at 0x160, paired with hitFlashTimer in the ctor
    // (strd r2, r1, [r4, #348] stores 0.0001f here) and rewritten to 0.001f in update().
    float field_0x160;
    int field_0x164;
    int field_0x168;
    int field_0x16c;

    PlayerAsteroid(int playerId, AEGeometry *geometry, int explosionType, int asteroidIndex,
                   const Vector &position, float scaling, int quality);

    ~PlayerAsteroid();

    void setAsteroidIndex(int asteroidIndex);

    void translate(const Vector &delta) override;

    void render() override;

    void setPosition(const Vector &position);

    int outerCollide(float x, float y, float z) override;

    int outerCollide(Vector point);

    Vector getPosition();

    void setRotationEnabled(bool enabled);

    int getQualityFrameIndex();

    int getQuality();

    float getScaling();

    uint8_t isMinable();

    String getQualityString();

    void update(int delta) override;

    Vector getProjectionVector(const Vector &value) override;

    void setAsteroidCenter(Vector center);

    int collide(float x, float y, float z) override;

    void push(int delta) override;

    void initPush(const Vector &target, int duration) override;

private:
    int &dropsLoot() { return this->hasCargo; }

    Array<int> *&loot() { return this->cargo; }

    AEGeometry *&secondaryGeometry() { return this->crateGeometry; }

    int &pushTimer() { return this->field_0x104; }
    int &pushDuration() { return *reinterpret_cast<int *>(reinterpret_cast<char *>(&this->field_0x104) + 4); }
    Vector &pushDirection() { return *reinterpret_cast<Vector *>(&this->field_0x10c); }
    Vector &pushSpin() { return *reinterpret_cast<Vector *>(&this->field_0x118); }

    static AbyssEngine::AEMath::Vector tmp_vector2;
    static AbyssEngine::AEMath::Vector asteroidCenter;
    static float asteroidDistance;
    static AbyssEngine::AEMath::Vector pos;
    static float emitTime;
};

#if __SIZEOF_POINTER__ == 4
static_assert(__builtin_offsetof(PlayerAsteroid, hitFlashTimer) == 0x15c, "PlayerAsteroid::hitFlashTimer offset");
static_assert(__builtin_offsetof(PlayerAsteroid, field_0x160) == 0x160, "PlayerAsteroid::field_0x160 offset");
static_assert(__builtin_offsetof(PlayerAsteroid, field_0x164) == 0x164, "PlayerAsteroid::field_0x164 offset");
static_assert(__builtin_offsetof(PlayerAsteroid, field_0x16c) == 0x16c, "PlayerAsteroid::field_0x16c offset");
static_assert(sizeof(PlayerAsteroid) == 0x170, "PlayerAsteroid size");
#endif
#endif
