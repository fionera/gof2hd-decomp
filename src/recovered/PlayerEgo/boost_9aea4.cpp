#include "class.h"
extern "C" void* g_boost_fmod;
extern "C" void FModSound_play(void*, void*, int, float);
void PlayerEgo_boost(PlayerEgo* self) {
  if (C(self, 0x13c) != 0) return;
  if (C(self, 0x146) == 0) return;
  if (I(self, 0x194) != 0) return;
  if (I(self, 0x138) < 0) return;
  float v = (float)I(self, 0xc8);
  I(self, 0x138) = 0;
  C(self, 0x13c) = 1;
  void* snd = *(void**)g_boost_fmod;
  F(self, 0xb8) = v;
  FModSound_play(*(void**)snd, P(self, 0xd4), 0, v);
}
