#ifndef GOF2_PLAYERSTATION_H
#define GOF2_PLAYERSTATION_H

#include <cstdint>

#include "mathtypes.h"
#include "game/ship/PlayerStaticFar.h"

class AEGeometry;
class Station;

class PlayerStation : public PlayerStaticFar {
public:
    using Vector = AbyssEngine::AEMath::Vector;

    AEGeometry *rootGeometry; // +0x140 owned root geometry (mesh tree)
    uint32_t meshTransform; // +0x144 sub-mesh transform handle (stationIndex 100 anim)
    int32_t stationIndex; // +0x148 station model index
    AEGeometry *secondGeometry; // +0x14c secondary owned geometry
    uint32_t collisionIndex; // +0x150 last colliding volume index
    int32_t collisionRadius; // +0x154 outer collision radius
    uint32_t field_0x158; // +0x158 zero-initialized, never read in this class (opaque)
    uint32_t field_0x15c; // +0x15c zero-initialized, never read in this class (opaque)
    uint32_t field_0x160; // +0x160 zero-initialized, never read in this class (opaque)
    uint32_t animTransform0; // +0x164 sub-mesh transform handle (stationIndex 0x6c)
    uint32_t animTransform1; // +0x168 sub-mesh transform handle
    uint32_t animTransform2; // +0x16c sub-mesh transform handle
    uint32_t animTransform3; // +0x170 sub-mesh transform handle

    explicit PlayerStation(Station *station);

    ~PlayerStation();

    void setVisible(bool visible);

    void setPosition(const Vector &position);

    Vector projectCollisionOnSurface(const Vector &position) override; // actor vtable slot +0x58
    void *getRoot();

    Vector getProjectionVector(const Vector &position) override; // actor vtable slot +0x50
    void render() override;

    int outerCollide(const Vector &position) override; // slot +0x40
    int outerCollide(float x, float y, float z) override; // slot +0x3c
    Vector getPosition();

    int collide(float x, float y, float z) override; // actor vtable slot +0x38
    void update(int delta) override;

    void translate(float x, float y, float z);
};

#endif
