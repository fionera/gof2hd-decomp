#include "class.h"
bool PlayerEgo_hasCloak(PlayerEgo* self) { return P(self, 0x1b0) != 0; }
