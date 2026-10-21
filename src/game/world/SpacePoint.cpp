#include "game/world/SpacePoint.h"

SpacePoint::SpacePoint(int type, const Vector &position, const Vector &direction, int param) {
    this->type = type;
    this->position = position;
    this->direction = direction;
    this->free = true;
    this->param = param;
}

void SpacePoint::giveFree() {
    this->free = true;
}

void SpacePoint::take() {
    this->free = false;
}

SpacePoint::~SpacePoint() {
}

bool SpacePoint::isFree() {
    return this->free;
}

int SpacePoint::getIndex() {
    return this->param;
}
