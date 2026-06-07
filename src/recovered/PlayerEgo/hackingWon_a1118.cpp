#include "class.h"
extern "C" int PlayerEgo_hackingWon_ext(int);
int PlayerEgo_hackingWon(PlayerEgo* self) {
  int v = I(self, 0x1e8);
  if (v == 0) return 0;
  return PlayerEgo_hackingWon_ext(v);
}
