#include "class.h"
extern "C" int PlayerEgo_isDead(PlayerEgo*);
extern "C" void stopShooting_extA(void*, int);
extern "C" void stopShooting_extB(void*, int, int);
void PlayerEgo_stopShooting(PlayerEgo* self, int param) {
  if (C(self, 0x1a0) != 0) {
    stopShooting_extA(P(self, 0), 2);
    return;
  }
  if (PlayerEgo_isDead(self) != 0) return;
  void* p = P(self, 0);
  if (param == 1) {
    stopShooting_extB(p, 1, I(self, 0x10c));
    return;
  }
  stopShooting_extA(p, param);
}
