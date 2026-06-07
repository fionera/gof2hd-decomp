#include "class.h"

using AbyssEngine::AEMath::Vector;

struct PlayerGasCloud {
    PlayerGasCloud(int param_1, ParticleSystemManager *param_2, AEGeometry *param_3,
                   Vector const &param_4);
};

extern "C" void *operator_new(uint32_t);
extern "C" void Player_ctor(void *self, int a, int hp, int b, int c, int d);
extern "C" void KIPlayer_ctor(void *self, int id, int faction, void *player, void *geom,
                              float x, float y, float z, bool flag);
extern "C" char PlayerGasCloud_vtable;
extern "C" void Player_setKIPlayer(void *player, void *ki);
extern "C" void Player_setMaxHitpoints(void *player, int hp);
extern "C" void Vector_assign(void *dst, Vector const &src);
extern "C" void AEGeometry_ctor(void *self, uint16_t meshId, void *canvas, bool b);
extern "C" void AEGeometry_setPosition_v(void *geom, Vector const &v);

__attribute__((visibility("hidden"))) extern void **g_pgc_canvas;

typedef int v4i __attribute__((__vector_size__(16), __aligned__(4)));
typedef int v4i1 __attribute__((__vector_size__(16), __aligned__(1)));

PlayerGasCloud::PlayerGasCloud(int param_1, ParticleSystemManager *param_2, AEGeometry *param_3,
                               Vector const &param_4)
{
    void *this_00 = operator_new(0x114);
    Player_ctor(this_00, 0, 9999999, 0, 0, 0);
    KIPlayer_ctor(this, param_1, -1, this_00, param_3, param_4.x, param_4.y, param_4.z, false);

    *(void **)this = &PlayerGasCloud_vtable + 8;
    F<void *>(this, 0x128) = 0;
    F<void *>(this, 0x12c) = 0;
    F<void *>(this, 0x130) = 0;
    Player_setKIPlayer(F<void *>(this, 0x4), this);
    Player_setMaxHitpoints(F<void *>(this, 0x4), 1);

    void **vt = *(void ***)this;
    typedef void (*pfn)(void *, Vector const &);
    ((pfn)vt[0x11])(this, param_4);

    F<int>(this, 0x158) = 0;
    F<int>(this, 0x78) = 0;
    F<uint8_t>(this, 0x25) = 0;

    int iVar1 = 0x4a35;
    if ((unsigned int)(param_1 - 0xc9) < 4)
        iVar1 = param_1 + 0x496c;
    int iVar2 = 0x4a39;
    if ((unsigned int)(param_1 - 0xc9) < 4)
        iVar2 = param_1 + 0x4970;
    F<int>(this, 0x160) = param_1;
    F<int>(this, 0x164) = iVar2;
    F<int>(this, 0x168) = iVar1;

    *(v4i1 *)((char *)this + 0x145) = (v4i1){0, 0, 0, 0};
    *(v4i *)((char *)this + 0x138) = (v4i){0, 0, 0, 0};

    Vector_assign((char *)this + 0x128, param_4);

    void *geom = operator_new(0xc0);
    AEGeometry_ctor(geom, F<uint16_t>(this, 0x168), *g_pgc_canvas, false);
    F<void *>(this, 0x134) = geom;
    AEGeometry_setPosition_v(geom, param_4);

    F<uint8_t>(this, 0x4c) = 1;
    F<uint8_t>(this, 0x44) = 1;
    F<int>(this, 0x88) = 0;
    F<uint8_t>(this, 0xf5) = 1;
    F<uint8_t>(this, 0x15c) = 0;
}
