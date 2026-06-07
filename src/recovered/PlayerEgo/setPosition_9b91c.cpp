#include "class.h"
extern "C" void AEGeometry_setPosition(void*, void*);
void PlayerEgo_setPosition3(PlayerEgo* self, float x, float y, float z) {
  void* g = P(self, 8);
  char v[12];
  *(float*)(v + 0) = x;
  *(float*)(v + 4) = y;
  *(float*)(v + 8) = z;
  AEGeometry_setPosition(g, v);
}
