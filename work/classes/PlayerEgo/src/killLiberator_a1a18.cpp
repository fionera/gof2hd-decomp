#include "class.h"
extern "C" void Vector_assign(void*, void*);
void PlayerEgo_killLiberator(PlayerEgo* self) {
  char sv[12];
  void* p = *(void**)P(self, 0);
  if (p == 0) return;
  void* arr = P(P(p, 4), 4);
  if (arr == 0) return;
  if (I(p, 0) == 0) return;
  if (I(arr, 0) == 0) return;
  if (I(self, 0x10c) != 0xb3) return;
  *(int*)(sv + 0) = -1; *(int*)(sv + 4) = -1; *(int*)(sv + 8) = -1;
  unsigned count = U(arr, 0);
  for (unsigned i = 0; i < count; i++) {
    int* e = (int*)(*(int*)((char*)arr + 4) + i * 4);
    if (e[0x16] == 0xb3) {
      *(int*)e[0xf] = -1;
      Vector_assign((void*)e[3], sv);
      void* arr2 = P(P(p, 4), 4);
      int* e2 = (int*)(*(int*)((char*)arr2 + 4) + i * 4);
      *((char*)e2 + 0x4c) = 0;
      count = U(arr2, 0);
    }
  }
}
