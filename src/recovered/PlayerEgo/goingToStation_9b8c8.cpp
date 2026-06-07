#include "class.h"
extern "C" void* Level_getLandmarks(void*);
bool PlayerEgo_goingToStation(PlayerEgo* self) {
  void* lm = Level_getLandmarks(P(self, 0xc));
  if (P(P(lm, 4), 0) == 0) return false;
  void* r4 = P(self, 0x15c);
  lm = Level_getLandmarks(P(self, 0xc));
  return r4 == P(P(lm, 4), 0);
}
