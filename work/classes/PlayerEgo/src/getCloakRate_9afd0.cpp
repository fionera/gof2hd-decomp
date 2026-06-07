#include "class.h"
float PlayerEgo_getCloakRate(PlayerEgo* self) {
  int off = 0x210;
  if (C(self, 0x1ac) == 0) off = 0x214;
  float a = (float)I(self, 0x208);
  float b = (float)I(self, off);
  float d = a / b;
  return d >= 0.0f ? d : 1.0f;
}
