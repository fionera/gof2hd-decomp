#include "class.h"

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
