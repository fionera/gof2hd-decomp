#ifndef GOF2_PLAYERSTATIC_H
#define GOF2_PLAYERSTATIC_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "mathtypes.h"
#include "game/ship/KIPlayer.h"

// Galaxy on Fire 2 -- PlayerStatic: a stationary KIPlayer that never moves. It
// stores an integer fallback position used when no render geometry is attached.

class AEGeometry;

class PlayerStatic : public KIPlayer {
public:
    using Vector = AbyssEngine::AEMath::Vector;

    int32_t positionX;   // integer fallback x position
    int32_t positionY;   // integer fallback y position
    int32_t positionZ;   // integer fallback z position

    PlayerStatic(int playerId, AEGeometry *geometry, float x, float y, float z);
    PlayerStatic(int playerId, AEGeometry *geometry);
    ~PlayerStatic();

    void render();
    Vector getPosition();
};

#endif
