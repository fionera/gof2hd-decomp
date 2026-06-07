#include "class.h"
bool PlayerEgo_isInDockingProcedure(PlayerEgo* self) {
  if (C(self, 0x1c0) != 0) return true;
  return C(self, 0x356) != 0;
}
