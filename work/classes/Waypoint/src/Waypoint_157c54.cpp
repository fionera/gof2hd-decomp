#include "class.h"

struct Waypoint {
    Waypoint(int x, int y, int z, Route *route);
};

void *operator new(__SIZE_TYPE__ size);
extern "C" void Player_ctor(Player *self, int radius, int hitpoints, int a, int b, int c);
extern "C" void KIPlayer_ctor(Waypoint *self, int kind, int team, Player *player,
                              AEGeometry *geometry, float x, float y, float z, bool flag);
extern "C" void Player_setActive(Player *self, bool active);

__attribute__((visibility("hidden"))) extern void *Waypoint_vtable;

Waypoint::Waypoint(int x, int y, int z, Route *route)
{
    Player *player = (Player *)operator new(0x114);
    Player_ctor(player, 2000, 0, 0, 0, 0);

    float zf = (float)z;
    float yf = (float)y;
    float xf = (float)x;
    KIPlayer_ctor(this, 0, -1, player, (AEGeometry *)0, xf, yf, zf, false);

    F<Route *>(this, 0x134) = route;
    F<void *>(this, 0x0) = (char *)Waypoint_vtable + 8;
    Player_setActive(F<Player *>(this, 0x4), false);

    F<int32_t>(this, 0x124) = x;
    F<int32_t>(this, 0x128) = y;
    F<int32_t>(this, 0x12c) = z;
    F<float>(this, 0x58) = xf;
    F<float>(this, 0x5c) = yf;
    F<float>(this, 0x60) = zf;
    F<uint16_t>(this, 0x130) = 0;
    F<uint8_t>(this, 0x72) = 1;
    F<uint8_t>(this, 0x4c) = 0;
}
