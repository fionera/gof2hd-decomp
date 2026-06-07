#include "class.h"

extern "C" void KIPlayer_setLevel(PlayerTurret *self, Level *level);
extern "C" int AEGeometry_getReferenceMatrix(AEGeometry *self);
extern "C" int ParticleSystemManager_addSystem(int manager, int matrix, int kind, int flags);
extern "C" void ParticleSystemManager_enableSystemEmit(int manager, int system, int enable);

struct PlayerTurret {
    void setLevel(Level *level);
};

void PlayerTurret::setLevel(Level *level)
{
    KIPlayer_setLevel(this, level);
    int manager = I(P(this, 0x54), 0x74);
    int matrix = AEGeometry_getReferenceMatrix(TP<AEGeometry>(this, 0x8));
    int system = ParticleSystemManager_addSystem(manager, matrix, 9, 0);
    I(this, 0x138) = system;
    ParticleSystemManager_enableSystemEmit(I(P(this, 0x54), 0x74), system, 0);
}
