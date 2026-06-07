#include "class.h"
void PlayerEgo_forceBoost(PlayerEgo* self) {
  I(self, 0x138) = 0;
  C(self, 0x13c) = 1;
  I(self, 0xb8) = 0x41000000;
  I(self, 0xcc) = 0x2710;
  I(self, 0xd0) = 0;
}
