#include "class.h"
float PlayerEgo_getDriveChargeRate(PlayerEgo* self) {
  float d = (float)I(self, 0x1fc) / (float)I(self, 0x200);
  return d >= 0.0f ? d : 1.0f;
}
