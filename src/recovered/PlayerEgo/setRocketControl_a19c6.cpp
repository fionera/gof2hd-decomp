#include "class.h"
extern "C" void* AEGeometry_getReferenceMatrix(void*);
extern "C" void ParticleSystemManager_systemSetMatrix(void*, int, void*);
extern "C" void PlayerEgo_setRocketControl_ext(PlayerEgo*, int);
void PlayerEgo_setRocketControl(PlayerEgo* self, void* gun, void* geo) {
  void* lvl = P(self, 0xc);
  I(self, 0x194) = (int)gun;
  int psm_arg = I(lvl, 0x64);
  void* psm = P(lvl, 0x88);
  if (gun == 0) {
    ParticleSystemManager_systemSetMatrix(psm, psm_arg, B(P(self, 0), 4));
    I(self, 0x198) = 0;
    return;
  }
  void* m = AEGeometry_getReferenceMatrix(geo);
  ParticleSystemManager_systemSetMatrix(psm, psm_arg, m);
  return PlayerEgo_setRocketControl_ext(self, 0);
}
