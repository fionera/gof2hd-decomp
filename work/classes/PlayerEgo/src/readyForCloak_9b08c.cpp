#include "class.h"
bool PlayerEgo_readyForCloak(PlayerEgo* self) {
  if (C(self, 0x1ad) == 0) return false;
  return I(self, 0x208) >= I(self, 0x214);
}
