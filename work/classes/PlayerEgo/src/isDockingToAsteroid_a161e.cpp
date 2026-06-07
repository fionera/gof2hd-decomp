#include "class.h"
bool PlayerEgo_isDockingToAsteroid(PlayerEgo* self) {
  if (C(self, 0x1c0) != 0) return I(self, 0x1c4) != 1;
  return false;
}
