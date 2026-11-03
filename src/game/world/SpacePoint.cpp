#include "game/world/SpacePoint.h"

SpacePoint::SpacePoint(int type, const Vector &position, const Vector &direction, int param) {
    this->type = type;
    this->position = position;
    this->direction = direction;
    this->free = true;
    this->param = param;
}

bool SpacePoint::isFree() {
    return this->free;
}

void SpacePoint::take() {
    this->free = false;
}

void SpacePoint::giveFree() {
    this->free = true;
}

int SpacePoint::getIndex() {
    return this->param;
}

SpacePoint::~SpacePoint() {
}
