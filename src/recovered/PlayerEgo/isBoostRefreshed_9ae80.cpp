#include "class.h"
int PlayerEgo_isBoostRefreshed(PlayerEgo* self) {
  if (C(self, 0x13c) != 0) return 0;
  if (C(self, 0x146) == 0) return 0;
  if (I(self, 0x138) > -1) return 1;
  return 0;
}
