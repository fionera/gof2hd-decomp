#include "class.h"
extern "C" void ParticleSystemManager_enableSystemEmit2(void*, int, int);
void PlayerEgo_setExhaustVisible(PlayerEgo* self, bool param) {
  void* lvl = P(self, 0xc);
  C(self, 0x32f) = param;
  C(P(lvl, 0x80), 0) = param;
  unsigned* arr = (unsigned*)P(lvl, 0xa8);
  if (arr != 0) {
    for (unsigned i = 0; i < arr[0]; i++) {
      int* p = (int*)arr[1];
      ParticleSystemManager_enableSystemEmit2(P(P(self, 0xc), 0x80), p[i], param);
    }
  }
}
