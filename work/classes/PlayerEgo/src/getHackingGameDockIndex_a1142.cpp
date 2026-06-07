#include "class.h"
extern "C" int PlayerEgo_getHackingGameDockIndex_ext(int);
int PlayerEgo_getHackingGameDockIndex(PlayerEgo* self) {
  int v = I(self, 0x1e8);
  if (v == 0) return -1;
  return PlayerEgo_getHackingGameDockIndex_ext(v);
}
