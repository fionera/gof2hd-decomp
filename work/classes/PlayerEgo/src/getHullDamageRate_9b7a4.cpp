#include "class.h"
extern "C" int PlayerEgo_getHullDamageRate_ext(void*);
int PlayerEgo_getHullDamageRate(PlayerEgo* self) { return PlayerEgo_getHullDamageRate_ext(P(self, 0)); }
