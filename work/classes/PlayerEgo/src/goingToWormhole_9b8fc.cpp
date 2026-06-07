#include "class.h"
extern "C" void* Level_getLandmarks(void*);
bool PlayerEgo_goingToWormhole(PlayerEgo* self) {
  void* m = P(self, 0xc);
  void* r4 = P(self, 0x15c);
  void* lm = Level_getLandmarks(m);
  return r4 == P(P(lm, 4), 0xc);
}
