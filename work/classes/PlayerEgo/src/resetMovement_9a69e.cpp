#include "class.h"
void PlayerEgo_resetMovement(PlayerEgo* self) {
  D(self, 0x278) = 0;
  D(self, 0x268) = 0;
  D(self, 0x270) = 0;
}
