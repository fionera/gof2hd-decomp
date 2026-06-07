#include "class.h"
bool PlayerEgo_isDockedToAsteroid(PlayerEgo* self) {
  if (C(self, 0x1c0) == 0) return false;
  return I(self, 0x1c4) == 1;
}
