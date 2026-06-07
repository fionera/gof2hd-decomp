#include "class.h"
extern void* g_appmgr;
extern "C" void* ApplicationManager_GetEngine(void*);
void PlayerEgo_setAutoPilot(PlayerEgo* self, void* kip) {
  C(self, 0x160) = 0;
  int v = (int)kip;
  I(self, 0x15c) = v;
  unsigned char old = C(self, 0x158);
  C(self, 0x158) = (v != 0) ? 1 : 0;
  if (v == 0) {
    I(P(self, 0x14), 0x2c) = 0;
    if (old != 0) {
      C(self, 0x2a8) = 0;
      I(self, 0x2a4) = 0;
    }
    return;
  }
  if (C(kip, 0x72) != 0) C(self, 0x160) = 1;
  void* eng = ApplicationManager_GetEngine(*(void**)g_appmgr);
  I(eng, 0x360) = 0;
  I(self, 0xbc) = 0x3f800000;
}
