#include "class.h"
extern "C" void* g_stopBoost_obj;
extern "C" void (*g_stopBoost_fn)(void*, int);
void PlayerEgo_stopBoost(PlayerEgo* self) {
  C(self, 0x13c) = 0;
  void* r4 = *(void**)g_stopBoost_obj;
  void (*fn)(void*, int) = g_stopBoost_fn;
  I(self, 0xb8) = 0x40000000;
  fn(*(void**)r4, 0x27);
  fn(*(void**)r4, 0x26);
  fn(*(void**)r4, 0x29);
  fn(*(void**)r4, 0x28);
  return fn(*(void**)r4, 0x44e);
}
