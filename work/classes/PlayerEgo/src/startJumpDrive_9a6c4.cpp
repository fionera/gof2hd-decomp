#include "class.h"
extern "C" void* g_FMod_singleton;
extern "C" void FModSound_play(void*, int, int, int, int);
extern "C" void Hud_hudEvent(int, int, PlayerEgo*, int);
void PlayerEgo_startJumpDrive(PlayerEgo* self) {
  if (C(self, 0x204) != 0) return;
  FModSound_play(*(void**)g_FMod_singleton, 0x21, 0, 0, 0);
  Hud_hudEvent(I(self, 0x220), 0x19, self, 0);
  I(self, 0x1fc) = 0;
  C(self, 0x204) = 1;
}
