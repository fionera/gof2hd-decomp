#ifndef GOF2_PLAYERSTATICFAR_H
#define GOF2_PLAYERSTATICFAR_H

#include <cstdint>
#include <vector>

#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "mathtypes.h"
#include "game/ship/PlayerStatic.h"

// Galaxy on Fire 2 -- PlayerStaticFar: a PlayerStatic that additionally projects
// distant objects onto a fixed-radius sphere around the camera so they stay
// visible (and scaled down) at long range.

class Player;
class AEGeometry;
class BoundingVolume;

class PlayerStaticFar : public PlayerStatic {
public:
    using Vector = AbyssEngine::AEMath::Vector;

    Player *player;                          // owning player instance
    Vector  initPosition;                    // original spawn position
    Vector  cameraPosition;                  // camera position captured each update
    Vector  objectPosition;                  // object position (float, from integer pos)
    Array<BoundingVolume*> *boundingVolumes;  // bounding-volume container
    Vector  viewDirection;                   // camera->object direction

    PlayerStaticFar(int playerId, AEGeometry *geometry, float x, float y, float z);
    ~PlayerStaticFar();

    Vector getProjectionVector(const Vector &value);
    void   render();
    Vector projectCollisionOnSurface(const Vector &value);
    bool   outerCollide(float x, float y, float z);
    void   outerCollide(Vector value);
    Vector getInitPosition();
    void   update(int delta);
    bool   collide(float x, float y, float z);
};

#endif
