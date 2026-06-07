#include "class.h"
extern "C" void PlayerEgo_setVisible_ext();
void PlayerEgo_setVisible(PlayerEgo* self, bool v) {
  C(self, 0x32e) = v;
  C(self, 0x309) = v;
  return PlayerEgo_setVisible_ext();
}
