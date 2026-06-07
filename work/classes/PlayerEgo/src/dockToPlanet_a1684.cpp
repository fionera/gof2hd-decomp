#include "class.h"
extern "C" void TargetFollowCamera_setLookAtCam(int);
extern "C" float PlayerEgo_resetGunDelay_f(PlayerEgo*);
extern "C" void* g_dockToPlanet_fmod;
extern "C" void FModSound_play(void*, void*, int, float, int);
void PlayerEgo_dockToPlanet(PlayerEgo* self) {
  TargetFollowCamera_setLookAtCam(I(self, 0x88));
  C(self, 0x13c) = 1;
  I(self, 0x138) = 0;
  I(self, 0xb8) = 0x41000000;
  I(self, 0xcc) = 10000;
  I(self, 0xd0) = 0;
  C(self, 0x144) = 0;
  float f = PlayerEgo_resetGunDelay_f(self);
  void* snd = *(void**)g_dockToPlanet_fmod;
  C(self, 0x38) = 0;
  C(self, 0x1ee) = 1;
  I(self, 0xb8) = 0x41000000;
  I(self, 0x1f0) = 0;
  FModSound_play(*(void**)snd, (void*)5, 0, f, 0);
}
