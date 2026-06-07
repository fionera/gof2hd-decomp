#include "class.h"
bool PlayerEgo_isLandingOrTakingOff(PlayerEgo* self) {
  if (C(self, 0x356) == 0) return false;
  return (U(self, 0x1c4) | 1) == 3;
}
