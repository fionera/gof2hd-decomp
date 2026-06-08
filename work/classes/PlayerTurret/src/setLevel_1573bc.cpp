#include "class.h"

extern "C" void KIPlayer_setLevel(PlayerTurret *self, Level *level);
extern "C" int AEGeometry_getReferenceMatrix(AEGeometry *self);
extern "C" int ParticleSystemManager_addSystem(int manager, int matrix, int kind, int flags);
extern "C" void ParticleSystemManager_enableSystemEmit(int manager, int system, int enable);

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

    void setLevel(Level *level);
};

void PlayerTurret::setLevel(Level *level)
{
    KIPlayer_setLevel(this, level);
    int manager = I(this->f_54, 0x74);
    int matrix = AEGeometry_getReferenceMatrix(TP<AEGeometry>(this, 0x8));
    int system = ParticleSystemManager_addSystem(manager, matrix, 9, 0);
    I(this, 0x138) = system;
    ParticleSystemManager_enableSystemEmit(I(this->f_54, 0x74), system, 0);
}
