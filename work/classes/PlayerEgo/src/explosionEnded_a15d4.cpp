#include "class.h"
bool PlayerEgo_explosionEnded(PlayerEgo* self) {
  if (I(self, 0x8c) == 0) return true;
  return 8000 < I(self, 0x2f8);
}
