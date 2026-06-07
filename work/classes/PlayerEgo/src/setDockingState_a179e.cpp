#include "class.h"
void PlayerEgo_setDockingState(PlayerEgo* self, int s) {
  if (s == 2 && I(self, 0x1c4) == 1) C(self, 0xb2) = 1;
  I(self, 0x1c4) = s;
}
