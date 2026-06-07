#include "class.h"
extern "C" void PlayerEgo_setTargetFollowCamera_ext(void*, void*);
void PlayerEgo_setTargetFollowCamera(PlayerEgo* self, void* cam) {
  void* m = P(self, 0x154);
  P(self, 0x88) = cam;
  return PlayerEgo_setTargetFollowCamera_ext(cam, m);
}
