#include "class.h"
bool PlayerEgo_driveReady(PlayerEgo* self) { return I(self, 0x1fc) >= I(self, 0x200); }
