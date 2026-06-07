#include "class.h"
int PlayerEgo_shouldSwitchToStandardCam(PlayerEgo* self) {
  if (C(self, 0xb0) == 0) return 0;
  C(self, 0xb0) = 0;
  return 1;
}
