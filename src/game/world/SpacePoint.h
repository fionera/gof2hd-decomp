#ifndef GOF2_SPACEPOINT_H
#define GOF2_SPACEPOINT_H

#include <cstdint>

#include "engine/math/Vector.h"

#include "engine/math/AEMath.h"

class SpacePoint {
public:
    AbyssEngine::AEMath::Vector position;
    AbyssEngine::AEMath::Vector direction;
    int type;
    bool free;
    int param;

    SpacePoint(int type, const AbyssEngine::AEMath::Vector &position,
               const AbyssEngine::AEMath::Vector &direction, int param);

    ~SpacePoint();

    void giveFree();

    void take();

    bool isFree();

    int getIndex();
};

#endif
