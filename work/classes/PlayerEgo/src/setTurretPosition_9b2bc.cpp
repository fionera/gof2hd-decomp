#include "class.h"
extern "C" void Vector_assign(void*, void*);
void PlayerEgo_setTurretPosition(PlayerEgo* self, float x, float y, float z) {
  char v[12];
  *(float*)(v + 0) = x;
  *(float*)(v + 4) = y;
  *(float*)(v + 8) = z;
  Vector_assign(B(self, 0x224), v);
}
