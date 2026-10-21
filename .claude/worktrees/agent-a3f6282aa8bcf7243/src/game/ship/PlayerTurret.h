#ifndef GOF2_PLAYERTURRET_H
#define GOF2_PLAYERTURRET_H

#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "mathtypes.h"
#include "game/ship/KIPlayer.h"

// Galaxy on Fire 2 - PlayerTurret: a KIPlayer subclass implementing sentry guns
// and ship-mounted turrets. It drives its own base/turret/helper geometry, picks
// the nearest valid enemy and rotates the barrel to track it.

class Player;
class AEGeometry;
class Explosion;
class Level;

class PlayerTurret : public KIPlayer {
public:
    using Vector = AbyssEngine::AEMath::Vector;

    // ---- PlayerTurret-specific state (stored in base-class padding slots in the
    // original 32-bit layout; modelled here as real members) ----
    bool                turretEnabled;      // turret tracking/firing active
    char                field_0x3e;         // set in ctor
    bool                isSentryGun;        // sentry-gun vs ship-turret discriminator
    Vector              cachedPosition;     // cached world position
    Vector              hostWorldOffset;    // host mount offset rotated into world space
    Vector              aimPoint;           // current aim/target point
    int                 reviveFlag;         // reset to 0 on revive

    int                 frameDelta;         // last update delta (ms)
    int                 spawnInvulnTimer;   // spawn invulnerability timer
    int                 explosionTimer;     // death/explosion timer
    int                 pickEnemyTimer;     // enemy re-acquisition / fire timer
    int                 rotationAccum;      // main-geometry rotation accumulator
    int                 particleSystemId;   // engine/death particle system id
    Explosion*          explosion;          // death explosion
    AEGeometry*         baseGeometry;       // base geometry
    AEGeometry*         turretGeometry;     // turret (barrel) geometry
    AEGeometry*         helperGeometry;     // helper/pivot geometry
    Player*             currentEnemy;       // current target
    Player*             previousEnemy;      // previous target
    KIPlayer*           turretHost;         // host KIPlayer (mounted ship)
    Vector              hostOffset;         // mount offset relative to host
    int                 turretRange;        // max acquisition range

    PlayerTurret(int mesh, Player* player, AEGeometry* geometry, float x, float y, float z);
    ~PlayerTurret();

    void setTurretRange(int range);
    void handleSentryGun(int delta);
    void setHost(KIPlayer* host, const Vector& offset);
    void render() override;
    int collide(float x, float y, float z) override;
    int outerCollide(float x, float y, float z) override;
    void handleTurret(int delta);
    void revive() override;
    void setPosition(const Vector& position);
    void reset();
    void setLevel(Level* level);
    KIPlayer* getHost();
    void setScaling(float scale);
    void handleRotation(int delta, AEGeometry* mainGeometry, AEGeometry* turretGeometry);
    void update(int delta) override;
    void pickEnemy();
};

#endif
