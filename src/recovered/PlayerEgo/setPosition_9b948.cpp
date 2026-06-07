#include "class.h"
extern "C" void PlayerEgo_setPosition_v(PlayerEgo*);
void PlayerEgo_setPosition(PlayerEgo* self) { return PlayerEgo_setPosition_v(self); }
