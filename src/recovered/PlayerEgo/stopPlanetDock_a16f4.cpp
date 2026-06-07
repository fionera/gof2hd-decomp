#include "class.h"
extern "C" void TargetFollowCamera_setLookAtCam(int, int);
extern "C" void PlayerEgo_stopBoost(PlayerEgo*);
void PlayerEgo_stopPlanetDock(PlayerEgo* self) {
  TargetFollowCamera_setLookAtCam(I(self, 0x88), 0);
  C(self, 0x144) = 1;
  C(self, 0x1ee) = 0;
  PlayerEgo_stopBoost(self);
  I(self, 0xb8) = 0x40000000;
}
