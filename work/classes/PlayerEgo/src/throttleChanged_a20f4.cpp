#include "class.h"
void PlayerEgo_throttleChanged(PlayerEgo* self) {
  int v;
  if (C(self, 0x370) == 0) {
    C(self, 0x370) = 1;
    v = 0;
  } else {
    v = I(self, 0x374);
    if ((unsigned)(v - 0x1f5) < 999) {
      v = 500;
    } else {
      if (v < 0x5dc) return;
      v = 2000 - v;
    }
  }
  I(self, 0x374) = v;
}
