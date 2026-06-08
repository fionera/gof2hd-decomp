#include "class.h"

extern "C" void AEGeometry_render(void *self);
extern "C" void Explosion_render(Explosion *self);
extern "C" void PlayerTurret_renderBase(PlayerTurret *self);

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

    void render();
};

void PlayerTurret::render()
{
    void *visible = this->f_78;
    if (visible != 0) {
        AEGeometry_render(visible);
    }
    int state = I(this, 0x88);
    if (state == 3) {
        Explosion_render(TP<Explosion>(this, 0x13c));
        state = I(this, 0x88);
    }
    if ((uint32_t)(state - 3) >= 2) {
        PlayerTurret_renderBase(this);
    }
}
