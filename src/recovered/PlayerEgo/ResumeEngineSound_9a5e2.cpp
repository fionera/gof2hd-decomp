#include "class.h"
extern "C" void PlayerEgo_ResumeEngineSound_ext(void*, int);
void PlayerEgo_ResumeEngineSound(PlayerEgo* self) { return PlayerEgo_ResumeEngineSound_ext(P(self, 0), 0); }
