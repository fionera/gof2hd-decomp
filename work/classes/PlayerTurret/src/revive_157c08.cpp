#include "class.h"

extern "C" void Player_reset(Player *self);
extern "C" void KIPlayer_setActive(PlayerTurret *self, bool active);
extern "C" void Explosion_reset(Explosion *self);
extern "C" void AEGeometry_setVisible(void *self, bool visible);

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

    void revive();
};

void PlayerTurret::revive()
{
    int zero = 0;
    int one = 1;
    Player_reset(TP<Player>(this, 0x4));
    *(volatile int *)B(this, 0x78) = zero;
    *(volatile int *)B(this, 0x88) = one;
    *(volatile int *)B(this, 0xd8) = zero;
    KIPlayer_setActive(this, true);
    Explosion_reset(TP<Explosion>(this, 0x13c));
    void *geometry = this->f_c;
    *(volatile int *)B(this, 0x128) = zero;
    *(volatile uint8_t *)B(this, 0xf5) = one;
    if (geometry == 0) {
        geometry = this->f_8;
    }
    AEGeometry_setVisible(geometry, true);
}
