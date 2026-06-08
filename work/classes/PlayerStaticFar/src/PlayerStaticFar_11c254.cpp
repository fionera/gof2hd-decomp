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
    this->f_134 = 0;
    this->f_138 = 0;
    this->f_13c = 0;
    this->f_58 = x;
    this->f_0 = (char *)vtable + 8;
    this->f_5c = y;
    this->f_60 = z;
    Player_setRadius(this->f_4, 0x1d4c);
    this->f_130 = 0;
}
