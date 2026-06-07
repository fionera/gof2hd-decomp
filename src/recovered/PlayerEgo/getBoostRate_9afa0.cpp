#include "class.h"
float PlayerEgo_getBoostRate(PlayerEgo* self) {
  float d = (float)I(self, 0x138) / (float)I(self, 0xd0);
  float r = d + 1.0f;
  if (0.0f < d) r = 1.0f;
  return r;
}
