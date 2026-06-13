#include "gof2/PlayerStatic.h"
#include "gof2/AEGeometry.h"
#include "gof2/Player.h"


extern "C" void *KIPlayer_dtor(PlayerStatic *self);
extern "C" void *PlayerStatic_dtor_tail();
extern "C" void PlayerStatic_render_geometry(void *geometry);

// ---- _PlayerStatic_e0970.cpp ----
void *_ZN12PlayerStaticD1Ev(PlayerStatic *self)
{
    KIPlayer_dtor(self);
    return PlayerStatic_dtor_tail();
}

// ---- ~PlayerStatic (D2 base-object destructor) ----
// PlayerStatic owns no members beyond the KIPlayer base; the non-deleting
// destructor simply chains into the KIPlayer base destructor. (The deleting
// variant _ZN12PlayerStaticD1Ev above additionally tail-calls operator delete.)
PlayerStatic::~PlayerStatic()
{
    KIPlayer_dtor(this);
}

// ---- render_e09b8.cpp ----
void PlayerStatic::render()
{
    return PlayerStatic_render_geometry(field_0x8);
}

// ---- render_geometry fragment ----
// Tail of PlayerStatic::render(): the resolved slot renders the owned geometry
// (the AEGeometry* passed in is this->geometry at +0x08).
void PlayerStatic::render_geometry(void *geometry)
{
    ((AEGeometry *)geometry)->render();
}

// ---- dtor_tail fragment ----
// Tail of the deleting destructor (_ZN12PlayerStaticD1Ev): after the KIPlayer
// base destructor runs, the deleting variant tail-calls operator delete(this).
// The fragment forwards the receiver to operator delete.
void *PlayerStatic::dtor_tail()
{
    ::operator delete(this);
    return this;
}

// ---- base_dtor / base_dtor_thunk fragments ----
// PlayerStatic base-object destructor used by the PlayerStaticFar destructor
// chain. PlayerStatic owns no members beyond the KIPlayer base, so it simply
// chains into the KIPlayer destructor and returns the receiver.
void *PlayerStatic::base_dtor()
{
    KIPlayer_dtor(this);
    return this;
}

// base_dtor_thunk: the b.w veneer that tail-calls the base destructor above.
void *PlayerStatic::base_dtor_thunk()
{
    return this->base_dtor();
}

// ---- ctor fragment ----
// PlayerStatic_ctor entry: the in-place construction body shared by the
// PlayerStaticFar constructor (it calls PlayerStatic_ctor on its own storage).
// Mirrors the full PlayerStatic(int, AEGeometry*, float, float, float) ctor.
__attribute__((visibility("hidden"))) extern void *volatile g_PlayerStatic_vtable;
void PlayerStatic::ctor(int playerId, AEGeometry *geometry, float x, float y, float z)
{
    Player *player = (Player *)operator new(0x114);
    ((Player *)(player))->ctor(2000, 0, 0, 0, 0);

    void *vtable = g_PlayerStatic_vtable;
    this->field_0x0 = (char *)vtable + 8;
    this->field_0x124 = (int)x;
    this->field_0x128 = (int)y;
    this->field_0x12c = (int)z;
}

// ---- getPosition_e0982.cpp ----
Vector PlayerStatic::getPosition()
{
    AEGeometry *geometry = field_0x8;
    if (geometry != 0) {
        return ((AEGeometry *)(geometry))->getPosition();
    }

    Vector result;
    result.x = (float)field_0x124;
    result.y = (float)field_0x128;
    result.z = (float)field_0x12c;
    return result;
}

// ---- PlayerStatic_e08cc.cpp ----
struct Player;


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

// ---- PlayerStatic(playerId, geometry) ----
// Convenience overload used when the caller (Level::createStaticActors) builds
// the actor at the origin and positions/rotates it afterwards. Delegates to the
// full constructor with a zero fallback position.
PlayerStatic::PlayerStatic(int playerId, AEGeometry *geometry)
    : PlayerStatic(playerId, geometry, 0.0f, 0.0f, 0.0f)
{
}
