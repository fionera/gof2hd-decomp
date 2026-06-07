#include "class.h"
extern "C" void* g_setRotation_transform;
extern "C" void* TransformGetLocal(void*, int);
extern "C" void MatrixSetRotation(void*, void*, float, float, float);
void PlayerEgo_setRotation(PlayerEgo* self, float rx, float ry, float rz) {
  char local[60];
  F(self, 0x2e8) = rx;
  F(self, 0x2ec) = ry;
  F(self, 0x2f0) = rz;
  void* t = *(void**)g_setRotation_transform;
  void* m = TransformGetLocal(t, I(P(self, 4), 0xc));
  MatrixSetRotation(local, m, F(self, 0x2e8), F(self, 0x2ec), F(self, 0x2f0));
}
