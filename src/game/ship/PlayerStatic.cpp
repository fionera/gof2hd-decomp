#include "game/ship/PlayerStatic.h"
#include "game/ship/KIPlayer.h"
#include "game/ship/Player.h"
#include "engine/render/AEGeometry.h"

// PlayerStatic(playerId, geometry, x, y, z): builds the underlying Player ship,
// constructs the KIPlayer base around it and records the integer fallback
// position used when no render geometry is attached.
PlayerStatic::PlayerStatic(int playerId, AEGeometry *geometry, float x, float y, float z)
    : KIPlayer(playerId, -1, new Player(2000, 0, 0, 0, 0), geometry, x, y, z, true)
{
    positionX = (int)x;
    positionY = (int)y;
    positionZ = (int)z;
}

// Convenience overload used when the caller (Level::createStaticActors) builds
// the actor at the origin and positions it afterwards.
PlayerStatic::PlayerStatic(int playerId, AEGeometry *geometry)
    : PlayerStatic(playerId, geometry, 0.0f, 0.0f, 0.0f)
{
}

// PlayerStatic owns no resources beyond the KIPlayer base; the base destructor
// (chained implicitly) releases the Player ship and geometry.
PlayerStatic::~PlayerStatic()
{
}

void PlayerStatic::render()
{
    geometry->render();
}

Vector PlayerStatic::getPosition()
{
    if (geometry != nullptr) {
        return geometry->getPosition();
    }

    Vector result;
    result.x = (float)positionX;
    result.y = (float)positionY;
    result.z = (float)positionZ;
    return result;
}
