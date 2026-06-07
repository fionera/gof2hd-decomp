#include "class.h"
extern "C" int Player_getCombinedHP(void*);
void PlayerEgo_resetLastHP(PlayerEgo* self) {
  I(self, 0x130) = Player_getCombinedHP(P(self, 0));
}
