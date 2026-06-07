#include "class.h"
extern "C" int PlayerEgo_getCurrentMiningAmount_ext(int);
int PlayerEgo_getCurrentMiningAmount(PlayerEgo* self) {
  int v = I(self, 0x1e4);
  if (v == 0) return 0;
  return PlayerEgo_getCurrentMiningAmount_ext(v);
}
