#include "class.h"
struct HackingGame { ~HackingGame(); };
void PlayerEgo_deleteHackingGame(PlayerEgo* self) {
  HackingGame* g = (HackingGame*)P(self, 0x1e8);
  if (g != 0) delete g;
  P(self, 0x1e8) = 0;
}
