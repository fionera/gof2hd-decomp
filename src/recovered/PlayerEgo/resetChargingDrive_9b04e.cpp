#include "class.h"
void PlayerEgo_resetChargingDrive(PlayerEgo* self) { UC(self, 0x204) = 0; }
