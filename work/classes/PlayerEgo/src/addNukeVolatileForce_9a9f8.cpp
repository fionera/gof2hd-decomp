#include "class.h"
void PlayerEgo_addNukeVolatileForce(PlayerEgo* self, float v) {
  void* p = P(self, 0);
  F(p, 0x60) = F(p, 0x60) + v * 3.0f;
}
