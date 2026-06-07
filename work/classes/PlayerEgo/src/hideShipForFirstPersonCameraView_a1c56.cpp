#include "class.h"
void PlayerEgo_hideShipForFirstPersonCameraView(PlayerEgo* self, bool param) {
  unsigned char r1 = param;
  C(self, 0x32d) = r1;
  unsigned char nr = r1 ^ 1;
  C(self, 0x309) = (C(self, 0x32e) != 0) & nr;
  *(char*)P(P(self, 0xc), 0x80) = nr & (C(self, 0x32f) != 0);
}
