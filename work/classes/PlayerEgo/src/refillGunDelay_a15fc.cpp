#include "class.h"
extern "C" void PlayerEgo_refillGunDelay_ext(void*, int);
void PlayerEgo_refillGunDelay(PlayerEgo* self) { return PlayerEgo_refillGunDelay_ext(P(self, 0), 0); }
