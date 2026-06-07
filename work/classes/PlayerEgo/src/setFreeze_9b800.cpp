#include "class.h"
void PlayerEgo_setFreeze(PlayerEgo* self, bool v) { C(self, 0x24) = v; }
