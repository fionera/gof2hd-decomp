#ifndef GOF2_PLAYERWORMHOLE_H
#define GOF2_PLAYERWORMHOLE_H

#include <cstdint>

#include "game/ship/PlayerStaticFar.h"

#include "engine/math/Vector.h"

class AEGeometry;

class PlayerWormHole : public PlayerStaticFar {
public:
    // ASM: isShrinking reads timer at [this,#336]=0x150; PlayerStaticFar ends @0x140 -> 16 bytes precede.
    uint8_t _pad_0x140[16];
    int timer;
    int scale;
    uint8_t missionLock;

    PlayerWormHole(int playerId, AEGeometry *geometry, float x, float y, float z, bool visible);

    ~PlayerWormHole();

    bool isShrinking();

    int open();

    Vector getPosition();

    void freeMissionLock();

    void render() override;

    void reset(bool shrinking);

    void setPosition(float x, float y, float z) override;

    void update(int elapsed) override;
};

#endif
