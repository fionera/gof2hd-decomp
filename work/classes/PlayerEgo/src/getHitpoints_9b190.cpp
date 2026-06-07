#include "class.h"
extern "C" int PlayerEgo_getHitpoints_ext(void*);
int PlayerEgo_getHitpoints(PlayerEgo* self) { return PlayerEgo_getHitpoints_ext(P(self, 0)); }
