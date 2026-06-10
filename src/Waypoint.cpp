#include "gof2/Waypoint.h"


extern "C" void Player_setActive(Player *self, bool active);
extern "C" void *KIPlayer_dtor(KIPlayer *self);
extern "C" void operator_delete(void *ptr);
extern "C" void Player_ctor(Player *self, int radius, int hitpoints, int a, int b, int c);

// ---- setActive_157d5e.cpp ----
struct Waypoint {
    void setActive(bool active);
};


void Waypoint::setActive(bool active)
{
    return Player_setActive(this->field_0x4, active);
}

// ---- reached_157d64.cpp ----
struct Waypoint {
    void reached();
};

void Waypoint::reached()
{
    this->field_0x130 = 0x101;
}

// ---- getPosition_157d38.cpp ----
struct Waypoint {
    Vector getPosition();
};

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
struct Waypoint {
    void reset();
};


void Waypoint::reset()
{
    Player *player = this->field_0x4;
    this->field_0x130 = 0;
    return Player_setActive(player, false);
}

// ---- Waypoint_157c54.cpp ----
struct Waypoint {
    Waypoint(int x, int y, int z, Route *route);
};

void *operator new(__SIZE_TYPE__ size);
extern "C" void KIPlayer_ctor(Waypoint *self, int kind, int team, Player *player,
                              AEGeometry *geometry, float x, float y, float z, bool flag);

__attribute__((visibility("hidden"))) extern void *Waypoint_vtable;

Waypoint::Waypoint(int x, int y, int z, Route *route)
{
    Player *player = (Player *)operator new(0x114);
    Player_ctor(player, 2000, 0, 0, 0, 0);

    float zf = (float)z;
    float yf = (float)y;
    float xf = (float)x;
    KIPlayer_ctor(this, 0, -1, player, (AEGeometry *)0, xf, yf, zf, false);

    this->field_0x134 = route;
    this->field_0x0 = (char *)Waypoint_vtable + 8;
    Player_setActive(this->field_0x4, false);

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
