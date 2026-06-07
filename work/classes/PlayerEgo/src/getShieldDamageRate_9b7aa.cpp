#include "class.h"
extern "C" int PlayerEgo_getShieldDamageRate_ext(void*);
int PlayerEgo_getShieldDamageRate(PlayerEgo* self) { return PlayerEgo_getShieldDamageRate_ext(P(self, 0)); }
