#include "class.h"
bool PlayerEgo_isDockedToDockingPoint(PlayerEgo* self) {
  if (C(self, 0x356) == 0) return false;
  return I(self, 0x1c4) == 1;
}
