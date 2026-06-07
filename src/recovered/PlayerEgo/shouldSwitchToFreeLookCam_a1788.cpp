#include "class.h"
int PlayerEgo_shouldSwitchToFreeLookCam(PlayerEgo* self) {
  if (C(self, 0xb1) == 0) return 0;
  C(self, 0xb1) = 0;
  return 1;
}
