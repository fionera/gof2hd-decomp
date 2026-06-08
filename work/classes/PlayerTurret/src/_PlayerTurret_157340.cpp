#include "class.h"

extern void *gPlayerTurretVtable __attribute__((visibility("hidden")));
extern "C" void *Explosion_dtor(Explosion *self);
extern "C" void *AEGeometry_dtor(AEGeometry *self);
extern "C" void PlayerTurret_operator_delete(void *ptr);

struct KIPlayer {
    ~KIPlayer() noexcept(false);
};

struct PlayerTurret {
    // @portable-fields
    void* f_0; // 0x0
    unsigned char _pad_4[4];
    void* f_8; // 0x8
    void* f_c; // 0xc
    unsigned char _pad_10[64];
    void* f_50; // 0x50
    void* f_54; // 0x54
    unsigned char _pad_58[32];
    void* f_78; // 0x78
    unsigned char _pad_7c[192];
    void* f_13c; // 0x13c
    void* f_140; // 0x140
    void* f_144; // 0x144
    void* f_148; // 0x148
    void* f_14c; // 0x14c
    void* f_150; // 0x150
    void* f_154; // 0x154
    void* f_158; // 0x158
    void* f_15c; // 0x15c

    ~PlayerTurret() noexcept(false);
};

PlayerTurret::~PlayerTurret() noexcept(false)
{
    this->f_0 = (char *)gPlayerTurretVtable + 8;

    Explosion *explosion = TP<Explosion>(this, 0x13c);
    if (explosion != 0) {
        PlayerTurret_operator_delete(Explosion_dtor(explosion));
    }
    this->f_13c = 0;

    AEGeometry *base = TP<AEGeometry>(this, 0x140);
    if (base != 0) {
        PlayerTurret_operator_delete(AEGeometry_dtor(base));
    }
    this->f_140 = 0;

    AEGeometry *turret = TP<AEGeometry>(this, 0x144);
    if (turret != 0) {
        PlayerTurret_operator_delete(AEGeometry_dtor(turret));
    }
    this->f_144 = 0;

    AEGeometry *helper = TP<AEGeometry>(this, 0x148);
    if (helper != 0) {
        PlayerTurret_operator_delete(AEGeometry_dtor(helper));
    }
    this->f_148 = 0;

    ((KIPlayer *)this)->~KIPlayer();
}
