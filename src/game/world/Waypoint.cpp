#include "game/world/Waypoint.h"
#include "game/ship/Player.h"

void Waypoint::setActive(bool active) {
    ((Player *) this->player)->setActive(active);
}

void Waypoint::reached() {
    this->state = 0x101;
}

Vector Waypoint::getPosition() {
    float x = (float) this->x;
    float y = (float) this->y;
    float z = (float) this->z;
    Vector result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

void Waypoint::reset() {
    reinterpret_cast<uint8_t &>(this->state) = 0; // ASM: original writes the low byte (strb), not strh
    ((Player *) this->player)->setActive(false);
}

Waypoint::Waypoint(int x, int y, int z, Route *route)
    : KIPlayer(0, -1, new Player(2000, 0, 0, 0, 0),
               nullptr, (float) x, (float) y, (float) z, false) {
    this->route = route;
    this->player->setActive(false);

    this->x = x;
    this->y = y;
    this->z = z;
    this->posX = (float) x;
    this->posY = (float) y;
    this->posZ = (float) z;
    this->state = 0;
    this->field_0x72 = 1;
    reinterpret_cast<uint8_t &>(this->hasCargo) = 0;
}

Waypoint::~Waypoint() {
}
