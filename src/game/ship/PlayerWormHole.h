#ifndef GOF2_PLAYERWORMHOLE_H
#define GOF2_PLAYERWORMHOLE_H

#include <cstdint>

#include "game/ship/PlayerStaticFar.h"

class AEGeometry;

class PlayerWormHole : public PlayerStaticFar {
public:
    using Vector = AbyssEngine::AEMath::Vector;

    int timer; // open/close animation timer (ms; <0 = opening)
    int scale; // current scale (12.4 fixed point, 0x1000 == 1.0)
    uint8_t missionLock; // keep this wormhole open for the active mission

    PlayerWormHole(int playerId, AEGeometry *geometry, float x, float y, float z, bool visible);

    ~PlayerWormHole();

    bool isShrinking();

    int open();

    Vector getPosition();

    void freeMissionLock();

    void render() override;

    void reset(bool shrinking);

    void setPosition(float x, float y, float z) override; // actor vtable slot +0x48
    void update(int elapsed) override;
};

#endif
