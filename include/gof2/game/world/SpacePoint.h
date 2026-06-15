#ifndef GOF2_SPACEPOINT_H
#define GOF2_SPACEPOINT_H

#include <cstdint>

#include "gof2/math.h"

// A point in space with a position, an orientation and an occupancy flag.
class SpacePoint {
public:
    AbyssEngine::AEMath::Vector position;
    AbyssEngine::AEMath::Vector direction;
    int type;
    uint8_t free;
    int param;

    SpacePoint(int type, const AbyssEngine::AEMath::Vector& position,
               const AbyssEngine::AEMath::Vector& direction, int param);

    void giveFree();
    void take();
};

#endif
