#ifndef GOF2_PLAYERSTATIC_H
#define GOF2_PLAYERSTATIC_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "game/ship/KIPlayer.h"

class AEGeometry;

class PlayerStatic : public KIPlayer {
public:
    using Vector = AbyssEngine::AEMath::Vector;

    int32_t positionX; // integer fallback x position
    int32_t positionY; // integer fallback y position
    int32_t positionZ; // integer fallback z position

    PlayerStatic(int playerId, AEGeometry *geometry, float x, float y, float z);

    ~PlayerStatic();

    void render() override;

    Vector getPosition();

    // A static actor never moves, ticks or takes part in collision: every
    // motion/collision virtual is overridden to do nothing (the binary emits a
    // bare stub for each one).
    void update(int dt) override;

    void translate(const Vector &v) override;

    int collide(float x, float y, float z) override;

    int outerCollide(float x, float y, float z) override;
};

#endif
