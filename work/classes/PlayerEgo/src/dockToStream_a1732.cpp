#include "class.h"
extern "C" void PlayerEgo_setPosition3(PlayerEgo*, float, float, float);
extern "C" void PlayerEgo_dockToStream_ext(PlayerEgo*, int);
void PlayerEgo_dockToStream(PlayerEgo* self, bool param) {
  if (param) { *(short*)B(self, 0x1ec) = 0x100; return; }
  I(self, 0xb8) = 0x40000000;
  PlayerEgo_setPosition3(self, F(self, 0xe0), F(self, 0xe4), F(self, 0xe8));
  C(self, 0x24) = 0;
  C(self, 0x145) = 0;
  *(short*)B(self, 0x1ec) = 0;
  return PlayerEgo_dockToStream_ext(self, 0);
}
