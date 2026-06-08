#include "class.h"

extern "C" void PlayerTurret_pickEnemy(PlayerTurret *self);
extern "C" void PlayerTurret_handleRotation(PlayerTurret *self, int delta, AEGeometry *main, AEGeometry *turret);

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

    void handleTurret(int delta);
};

void PlayerTurret::handleTurret(int delta)
{
    I(this, 0x130) = I(this, 0x130) + delta;
    PlayerTurret_pickEnemy(this);
    void *enemy = this->f_14c;
    if (enemy != 0 && UC(enemy, 0x5e) == 0) {
        PlayerTurret_handleRotation(this, delta, TP<AEGeometry>(this, 0x148), TP<AEGeometry>(this, 0x144));
    }
}
