#include "class.h"
extern "C" void PlayerEgo_hitCamera_ext(int);
void PlayerEgo_hitCamera(PlayerEgo* self) {
  C(self, 0x32c) = 1;
  int cam = I(self, 0x88);
  I(self, 0x328) = 0;
  return PlayerEgo_hitCamera_ext(cam);
}
