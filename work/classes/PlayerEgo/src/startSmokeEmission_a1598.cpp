#include "class.h"
extern "C" void ParticleSystemManager_enableSystemEmit(void*, int, int);
extern "C" void PlayerEgo_startSmokeEmission_ext(void*, int, int);
void PlayerEgo_startSmokeEmission(PlayerEgo* self) {
  int v = I(self, 0x300);
  if (v < 0) return;
  ParticleSystemManager_enableSystemEmit(P(P(self, 0xc), 0x78), v, 1);
  PlayerEgo_startSmokeEmission_ext(P(P(self, 0xc), 0x84), I(self, 0x304), 1);
}
