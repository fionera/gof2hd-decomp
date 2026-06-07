#include "class.h"
extern "C" void PlayerEgo_resetGunDelay_ext(void*, int);
void PlayerEgo_resetGunDelay(PlayerEgo* self) { return PlayerEgo_resetGunDelay_ext(P(self, 0), 0); }
