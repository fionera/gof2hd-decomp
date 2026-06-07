#include "class.h"
bool PlayerEgo_isDockingToDockingPoint(PlayerEgo* self) {
  if (C(self, 0x356) != 0) return I(self, 0x1c4) != 1;
  return false;
}
