#include "class.h"
bool PlayerEgo_isHacking(PlayerEgo* self) { return I(self, 0x1e8) != 0; }
