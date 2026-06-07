#include "class.h"
extern "C" void PlayerEgo_setActive_ext(void*);
void PlayerEgo_setActive(PlayerEgo* self, bool) { return PlayerEgo_setActive_ext(P(self, 0)); }
