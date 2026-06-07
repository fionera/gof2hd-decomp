#include "class.h"
extern "C" int PlayerEgo_getHitpoints(PlayerEgo*);
bool PlayerEgo_isDead(PlayerEgo* self) { return PlayerEgo_getHitpoints(self) < 1; }
