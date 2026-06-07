#include "class.h"
extern "C" void* AEGeometry_getMatrix(void*);
extern "C" void Matrix_mul(void*, void*, void*);
extern "C" void MatrixGetPosition(void*, void*);
void PlayerEgo_getTurretPosition(void* out, void* src) {
  char local[60];
  void* m1 = AEGeometry_getMatrix(P(src, 8));
  void* m2 = AEGeometry_getMatrix(P(src, 0x28));
  Matrix_mul(local, m1, m2);
  MatrixGetPosition(out, local);
}
