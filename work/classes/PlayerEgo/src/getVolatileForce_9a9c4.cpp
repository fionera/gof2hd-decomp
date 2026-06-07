#include "class.h"
float PlayerEgo_getVolatileForce(PlayerEgo* self) {
  float f = F(P(self, 0), 0x60);
  float r = 0.0f;
  if (!(f < 0.0f)) {
    r = 1.0f;
    if (!(f > 1.0f)) r = f;
  }
  return r;
}
