#include "class.h"
extern "C" void PlayerEgo_setAutoTurret_ext(void*, int);
void PlayerEgo_setAutoTurret(PlayerEgo* self, bool on) {
  C(self, 0x355) = on;
  if (!on) PlayerEgo_setAutoTurret_ext(P(self, 0), 2);
}
