#include "PlayerStatic.h"


extern "C" void *KIPlayer_dtor(PlayerStatic *self);
extern "C" void *PlayerStatic_dtor_tail();
extern "C" void PlayerStatic_render_geometry(void *geometry);
extern "C" Vector AEGeometry_getPosition(AEGeometry *geometry);

// ---- _PlayerStatic_e0970.cpp ----
extern "C" void *_ZN12PlayerStaticD1Ev(PlayerStatic *self)
{
    KIPlayer_dtor(self);
    return PlayerStatic_dtor_tail();
}

// ---- render_e09b8.cpp ----
void PlayerStatic::render()
{
    return PlayerStatic_render_geometry(pp(this, 0x8));
}

// ---- getPosition_e0982.cpp ----
Vector PlayerStatic::getPosition()
{
    AEGeometry *geometry = (AEGeometry *)pp(this, 0x8);
    if (geometry != 0) {
        return AEGeometry_getPosition(geometry);
    }

    Vector result;
    result.x = (float)i32(this, 0x124);
    result.y = (float)i32(this, 0x128);
    result.z = (float)i32(this, 0x12c);
    return result;
}

// ---- PlayerStatic_e08cc.cpp ----
struct Player;

void *operator new(__SIZE_TYPE__ size);

extern "C" void Player_ctor(Player *self, int hitpoints, int armor, int shield,
                            int emp, int gamma);
extern "C" void KIPlayer_ctor(PlayerStatic *self, int playerId, int group, Player *player,
                              AEGeometry *geometry, float x, float y, float z, int flag);
__attribute__((visibility("hidden"))) extern void *volatile g_PlayerStatic_vtable;

PlayerStatic::PlayerStatic(int playerId, AEGeometry *geometry, float x, float y, float z)
{
    Player *player = (Player *)operator new(0x114);
    Player_ctor(player, 2000, 0, 0, 0, 0);
    KIPlayer_ctor(this, playerId, -1, player, geometry, x, y, z, 0);

    void *vtable = g_PlayerStatic_vtable;
    this->f_0 = (char *)vtable + 8;
    this->f_124 = (int)x;
    this->f_128 = (int)y;
    this->f_12c = (int)z;
}
