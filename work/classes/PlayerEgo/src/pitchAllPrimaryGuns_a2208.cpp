#include "class.h"
extern "C" void PlayerEgo_pitchAllPrimaryGuns_ext(void*);
void PlayerEgo_pitchAllPrimaryGuns(PlayerEgo* self, float) { return PlayerEgo_pitchAllPrimaryGuns_ext(P(self, 0)); }
