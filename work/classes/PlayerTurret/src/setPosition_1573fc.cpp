#include "class.h"

using AbyssEngine::AEMath::Vector;

extern "C" void AEGeometry_setPosition(AEGeometry *self, const Vector *position);

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

    void setPosition(const Vector &position);
};

void PlayerTurret::setPosition(const Vector &position)
{
    AEGeometry_setPosition(TP<AEGeometry>(this, 0x8), &position);
    I(this, 0x58) = *(const int *)B(&position, 0x0);
    I(this, 0x5c) = *(const int *)B(&position, 0x4);
    I(this, 0x60) = *(const int *)B(&position, 0x8);
}
