#include "gof2/Waypoint.h"
#include "gof2/Player.h"


extern "C" void *KIPlayer_dtor(KIPlayer *self);

__attribute__((visibility("hidden"))) extern void *Waypoint_vtable;

// ---- setActive_157d5e.cpp ----
void Waypoint::setActive(bool active)
{
    return ((Player *)(this->player))->setActive(active);
}

// ---- reached_157d64.cpp ----
void Waypoint::reached()
{
    this->state = 0x101;
}

// ---- getPosition_157d38.cpp ----
Vector Waypoint::getPosition()
{
    float x = (float)this->x;
    float y = (float)this->y;
    float z = (float)this->z;
    Vector result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

// ---- _Waypoint_157d28.cpp ----
void _ZN8WaypointD0Ev(Waypoint *self)
{
    return ::operator delete(KIPlayer_dtor((KIPlayer *)self));
}

// ---- ~Waypoint (base/complete-object destructor) at 0x167d28 ----
// Waypoint extends KIPlayer (see the ctor, which constructs the KIPlayer subobject
// in place). The destructor simply runs the KIPlayer base destructor; the held
// Player object is owned/torn down by KIPlayer. Expressed through the KIPlayer_dtor
// shim, matching the deleting destructor above and keeping this TU self-contained.
Waypoint::~Waypoint()
{
    KIPlayer_dtor((KIPlayer *)this);
}

// ---- reset_157d6e.cpp ----
void Waypoint::reset()
{
    Player *player = this->player;
    this->state = 0;
    return ((Player *)(player))->setActive(false);
}

// ---- Waypoint_157c54.cpp ----
Waypoint::Waypoint(int x, int y, int z, Route *route)
{
    Player *player = (Player *)operator new(0x114);
    ((Player *)(player))->ctor(2000, 0, 0, 0, 0);

    float zf = (float)z;
    float yf = (float)y;
    float xf = (float)x;

    this->route = route;
    this->field_0x0 = (char *)Waypoint_vtable + 8;
    ((Player *)(this->player))->setActive(false);

    this->x = x;
    this->y = y;
    this->z = z;
    this->fx = xf;
    this->fy = yf;
    this->fz = zf;
    this->state = 0;
    this->field_0x72 = 1;
    this->field_0x4c = 0;
}
