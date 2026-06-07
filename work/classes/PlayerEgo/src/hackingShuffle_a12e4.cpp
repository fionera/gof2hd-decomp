#include "class.h"
extern "C" void PlayerEgo_hackingShuffle_ext(int);
void PlayerEgo_hackingShuffle(PlayerEgo* self) {
  int v = I(self, 0x1e8);
  if (v != 0) PlayerEgo_hackingShuffle_ext(v);
}
