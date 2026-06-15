#include "gof2/game/world/SpacePoint.h"

using AbyssEngine::AEMath::Vector;

SpacePoint::SpacePoint(int type, const Vector& position, const Vector& direction, int param)
    : position(position), direction(direction), type(type), free(1), param(param) {}

void SpacePoint::giveFree() {
    this->free = 1;
}

void SpacePoint::take() {
    this->free = 0;
}
