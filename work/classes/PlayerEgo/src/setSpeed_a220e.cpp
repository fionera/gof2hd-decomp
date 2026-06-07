#include "class.h"
void PlayerEgo_setSpeed(PlayerEgo* self, float v) { F(self, 0xb8) = v; }
