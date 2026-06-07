#include "class.h"
extern "C" int PlayerEgo_getHitpoints(PlayerEgo*);
bool PlayerEgo_isInWormhole(PlayerEgo* self) {
  if (PlayerEgo_getHitpoints(self) > 0) return C(self, 0x25) != 0;
  return false;
}
