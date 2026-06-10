#include "gof2/PlayerStatic.h"
#include "gof2/Player.h"


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
    return PlayerStatic_render_geometry(field_0x8);
}

// ---- getPosition_e0982.cpp ----
Vector PlayerStatic::getPosition()
{
    AEGeometry *geometry = field_0x8;
    if (geometry != 0) {
        return AEGeometry_getPosition(geometry);
    }

    Vector result;
    result.x = (float)field_0x124;
    result.y = (float)field_0x128;
    result.z = (float)field_0x12c;
    return result;
}

// ---- PlayerStatic_e08cc.cpp ----
struct Player;

void *operator new(__SIZE_TYPE__ size);

__attribute__((visibility("hidden"))) extern void *volatile g_PlayerStatic_vtable;

PlayerStatic::PlayerStatic(int playerId, AEGeometry *geometry, float x, float y, float z)
{
    Player *player = (Player *)operator new(0x114);
    ((Player *)(player))->ctor(2000, 0, 0, 0, 0);

    void *vtable = g_PlayerStatic_vtable;
    this->field_0x0 = (char *)vtable + 8;
    this->field_0x124 = (int)x;
    this->field_0x128 = (int)y;
    this->field_0x12c = (int)z;
}
