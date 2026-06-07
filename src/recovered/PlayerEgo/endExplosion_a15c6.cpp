#include "class.h"
extern "C" void PlayerEgo_endExplosion_ext(int);
void PlayerEgo_endExplosion(PlayerEgo* self) {
  int v = I(self, 0x8c);
  if (v != 0) PlayerEgo_endExplosion_ext(v);
}
