#include "class.h"
void PlayerEgo_changeThrust(PlayerEgo* self, float v) {
  float n = F(self, 0xbc) + v;
  float p3 = 1.0f;
  if (n < 1.0f) p3 = 0.0f;
  if (n < 0.0f) p3 = 0.0f;
  float p2 = p3;
  if (n < 1.0f) p2 = n;
  if (n < 0.0f) p2 = p3;
  F(self, 0xbc) = p2;
}
