#include "class.h"
// 000aa5dc: ldr r0,[r0,#0x0] ; b.w <ext>   — tail-call ext(self->m_pPlayer)
extern "C" void PlayerEgo_PauseEngineSound_ext(void*);
void PlayerEgo_PauseEngineSound(PlayerEgo* self) {
  return PlayerEgo_PauseEngineSound_ext(P(self, 0));
}
