#include "class.h"

extern void *gPlayerTurretVtable __attribute__((visibility("hidden")));
extern "C" void *Explosion_dtor(Explosion *self);
extern "C" void *AEGeometry_dtor(AEGeometry *self);
extern "C" void PlayerTurret_operator_delete(void *ptr);

struct KIPlayer {
    ~KIPlayer() noexcept(false);
};

struct PlayerTurret {
    ~PlayerTurret() noexcept(false);
};

PlayerTurret::~PlayerTurret() noexcept(false)
{
    P(this, 0x0) = (char *)gPlayerTurretVtable + 8;

    Explosion *explosion = TP<Explosion>(this, 0x13c);
    if (explosion != 0) {
        PlayerTurret_operator_delete(Explosion_dtor(explosion));
    }
    P(this, 0x13c) = 0;

    AEGeometry *base = TP<AEGeometry>(this, 0x140);
    if (base != 0) {
        PlayerTurret_operator_delete(AEGeometry_dtor(base));
    }
    P(this, 0x140) = 0;

    AEGeometry *turret = TP<AEGeometry>(this, 0x144);
    if (turret != 0) {
        PlayerTurret_operator_delete(AEGeometry_dtor(turret));
    }
    P(this, 0x144) = 0;

    AEGeometry *helper = TP<AEGeometry>(this, 0x148);
    if (helper != 0) {
        PlayerTurret_operator_delete(AEGeometry_dtor(helper));
    }
    P(this, 0x148) = 0;

    ((KIPlayer *)this)->~KIPlayer();
}
