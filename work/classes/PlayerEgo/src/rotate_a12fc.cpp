#include "class.h"
extern "C" void* g_rotate_transform;
extern "C" void* TransformGetLocal(void*, int);
extern "C" void MatrixSetRotation(void*, void*, float, float, float);
void PlayerEgo_rotate(PlayerEgo* self, float rx, float ry, float rz) {
  char local[60];
  F(self, 0x2e8) = F(self, 0x2e8) + rx;
  F(self, 0x2ec) = F(self, 0x2ec) + ry;
  F(self, 0x2f0) = F(self, 0x2f0) + rz;
  int ido = I(P(self, 4), 0xc);
  void* t = *(void**)g_rotate_transform;
  void* m = TransformGetLocal(t, ido);
  MatrixSetRotation(local, m, F(self, 0x2e8), F(self, 0x2ec), F(self, 0x2f0));
}
