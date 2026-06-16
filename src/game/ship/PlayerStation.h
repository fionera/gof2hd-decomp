#ifndef GOF2_PLAYERSTATION_H
#define GOF2_PLAYERSTATION_H

#include <cstdint>

#include "mathtypes.h"
#include "game/ship/PlayerStaticFar.h"

class AEGeometry;
class Station;

// Galaxy on Fire 2 -- PlayerStation: a stationary station object. It is a
// PlayerStaticFar whose render geometry is a (possibly multi-mesh, animated)
// station model selected from the station index, with a collision-volume list
// loaded from the station's *.bin collision data.
class PlayerStation : public PlayerStaticFar {
public:
    using Vector = AbyssEngine::AEMath::Vector;

    AEGeometry *rootGeometry;       // owned root geometry (mesh tree)
    uint32_t    meshTransform;      // sub-mesh transform handle (stationIndex 100 anim)
    int32_t     stationIndex;       // station model index
    AEGeometry *secondGeometry;     // secondary owned geometry
    uint32_t    collisionIndex;     // last colliding volume index
    int32_t     collisionRadius;    // outer collision radius
    uint32_t    field_0x158;
    uint32_t    field_0x15c;
    uint32_t    field_0x160;
    uint32_t    animTransform0;     // sub-mesh transform handle (stationIndex 0x6c)
    uint32_t    animTransform1;     // sub-mesh transform handle
    uint32_t    animTransform2;     // sub-mesh transform handle
    uint32_t    animTransform3;     // sub-mesh transform handle

    explicit PlayerStation(Station *station);
    ~PlayerStation();

    void   setVisible(bool visible);
    void   setPosition(const Vector &position);
    Vector projectCollisionOnSurface(const Vector &position);
    void  *getRoot();
    Vector getProjectionVector(const Vector &position);
    void   render();
    void   outerCollide(const Vector &position);
    bool   outerCollide(float x, float y, float z);
    Vector getPosition();
    bool   collide(float x, float y, float z);
    void   update(int delta);
    void   translate(float x, float y, float z);
};

#endif
