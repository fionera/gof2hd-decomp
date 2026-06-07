#include "class.h"
float PlayerEgo_getCloakingPercentage(PlayerEgo* self) {
  float r = 0.0f;
  if (C(self, 0x1ac) != 0) {
    int v = I(self, 0x208);
    if (v >= 0) {
      if (v < 2000) {
        r = (float)v * 100.0f / 2000.0f;
      } else {
        int lo = I(self, 0x210) - 2000;
        if (v > lo) {
          r = (((float)v - (float)lo) / -2000.0f + 1.0f) * 100.0f;
        } else {
          r = 100.0f;
        }
      }
    }
  }
  return r;
}
