#include "class.h"

// Base PlayerStatic constructor (blx 0x75fdc) and Player::setRadius (blx 0x730d8).
extern "C" void PlayerStatic_ctor(PlayerStaticFar *self, int playerId,
                                  AEGeometry *geometry, float x, float y, float z);
extern "C" void Player_setRadius(void *player, int value);

// Hidden-visibility vtable so the address is materialized via a PC-relative load.
__attribute__((visibility("hidden"))) extern void *volatile g_PlayerStaticFar_vtable;

PlayerStaticFar::PlayerStaticFar(int playerId, AEGeometry *geometry, float x, float y, float z)
{
    PlayerStatic_ctor(this, playerId, geometry, x, y, z);

    void *vtable = g_PlayerStaticFar_vtable;
    i32(this, 0x134) = 0;
    i32(this, 0x138) = 0;
    i32(this, 0x13c) = 0;
    f32(this, 0x58) = x;
    pp(this, 0x0) = (char *)vtable + 8;
    f32(this, 0x5c) = y;
    f32(this, 0x60) = z;
    Player_setRadius(pp(this, 0x4), 0x1d4c);
    i32(this, 0x130) = 0;
}
