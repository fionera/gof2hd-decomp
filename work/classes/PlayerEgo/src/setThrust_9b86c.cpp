#include "class.h"
void PlayerEgo_setThrust(PlayerEgo* self, float v) { F(self, 0xbc) = v; }
