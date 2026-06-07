#include "class.h"
extern "C" int __aeabi_idiv(int, int);
float PlayerEgo_getBoostPercentage(PlayerEgo* self) {
  float den = (float)__aeabi_idiv(I(self, 0xcc), 6);
  float d = (float)I(self, 0x138) / den;
  float r;
  if (d < 1.0f) {
    r = d;
  } else {
    float sub = 6.0f - d;
    r = 1.0f;
    if (5.0f < d) r = sub;
  }
  return r;
}
