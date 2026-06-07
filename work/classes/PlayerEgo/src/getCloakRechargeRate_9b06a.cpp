#include "class.h"
float PlayerEgo_getCloakRechargeRate(PlayerEgo* self) {
  return 1.0f - (float)I(self, 0x20c) / (float)I(self, 0x368);
}
