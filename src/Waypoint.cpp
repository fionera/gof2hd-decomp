#include "gof2/Waypoint.h"
#include "gof2/Player.h"


extern "C" void *KIPlayer_dtor(KIPlayer *self);
extern "C" void operator_delete(void *ptr);
void *operator new(__SIZE_TYPE__ size);

__attribute__((visibility("hidden"))) extern void *Waypoint_vtable;

// ---- setActive_157d5e.cpp ----
void Waypoint::setActive(bool active)
{
    return ((Player *)(this->field_0x4))->setActive(active);
}

// ---- reached_157d64.cpp ----
void Waypoint::reached()
{
    this->field_0x130 = 0x101;
}

// ---- getPosition_157d38.cpp ----
Vector Waypoint::getPosition()
{
    float x = (float)this->field_0x124;
    float y = (float)this->field_0x128;
    float z = (float)this->field_0x12c;
    Vector result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

// ---- _Waypoint_157d28.cpp ----
extern "C" void _ZN8WaypointD0Ev(Waypoint *self)
{
    return operator_delete(KIPlayer_dtor((KIPlayer *)self));
}

// ---- reset_157d6e.cpp ----
void Waypoint::reset()
{
    Player *player = this->field_0x4;
    this->field_0x130 = 0;
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

    this->field_0x134 = route;
    this->field_0x0 = (char *)Waypoint_vtable + 8;
    ((Player *)(this->field_0x4))->setActive(false);

    this->field_0x124 = x;
    this->field_0x128 = y;
    this->field_0x12c = z;
    this->field_0x58 = xf;
    this->field_0x5c = yf;
    this->field_0x60 = zf;
    this->field_0x130 = 0;
    this->field_0x72 = 1;
    this->field_0x4c = 0;
}
