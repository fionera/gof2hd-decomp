#ifndef GOF2_PLAYERJUNK_H
#define GOF2_PLAYERJUNK_H
#include "game/ship/KIPlayer.h"

// Galaxy on Fire 2 - PlayerJunk: a destructible piece of space junk.
//
// PlayerJunk is a behaviour-only specialisation of KIPlayer: it adds no data of
// its own and reuses the inherited KIPlayer fields (player, cargo, level, state,
// crateGeometry, ...). When its underlying Player is destroyed it plays a death
// sound, may drop a crate, detaches from the player target and emits a wreck
// particle burst.

class Player;
class AEGeometry;

class PlayerJunk : public KIPlayer {
public:
    PlayerJunk(int type, Player* player, AEGeometry* geometry, float x, float y, float z);
    ~PlayerJunk();

    void update(int elapsed) override;
    void reset();
    void render() override;
};
#endif
