#include "gof2/game/world/Waypoint.h"
#include "gof2/game/ship/Player.h"

void Waypoint::setActive(bool active)
{
    ((Player*)this->player)->setActive(active);
}

void Waypoint::reached()
{
    this->state = 0x101;
}

// Tail-call target of Route::reachWaypoint: once the route has stepped past this
// waypoint it is flagged as reached.
void Waypoint::activate()
{
    this->reached();
}

// Tail-call target of Route::update: when the ship gets close enough the route makes
// the next waypoint active. The float return is the unused passthrough Route::update
// hands back as its result.
float Waypoint::advance(bool active)
{
    this->setActive(active);
    return 0.0f;
}

Vector Waypoint::getPosition()
{
    Vector result;
    result.x = (float)this->x;
    result.y = (float)this->y;
    result.z = (float)this->z;
    return result;
}

void Waypoint::reset()
{
    this->state = 0;
    ((Player*)this->player)->setActive(false);
}

Waypoint::Waypoint(int x, int y, int z, Route* route)
{
    Player* player = (Player*)::operator new(sizeof(Player));
    player->ctor(2000, 0, 0, 0, 0);

    float fx = (float)x;
    float fy = (float)y;
    float fz = (float)z;

    this->KIPlayer::ctor(0, -1, player, nullptr, fx, fy, fz, false);

    this->route = route;
    ((Player*)this->player)->setActive(false);

    this->x = x;
    this->y = y;
    this->z = z;
    this->posX = fx;
    this->posY = fy;
    this->posZ = fz;
    this->state = 0;
    this->field_0x72 = 1;
    this->field_0x4c = 0;
}
