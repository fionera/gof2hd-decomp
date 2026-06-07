#include "class.h"
extern "C" void ParticleSystemManager_enableSystemEmit3(void*, int, int);
extern "C" void TargetFollowCamera_setActive(void*, int);
extern "C" void* operator_new_(unsigned);
extern "C" void Explosion_ctor(void*, int);
extern "C" void Player_setActive_(int);
extern void* g_explode_obj;
extern void (*g_explode_fn)(void*, int);
extern "C" void PlayerEgo_explode_ext(PlayerEgo*, int);
void PlayerEgo_explode(PlayerEgo* self) {
  ParticleSystemManager_enableSystemEmit3(P(P(self, 0xc), 0x74), I(self, 0x2fc), 1);
  if (I(self, 0x8c) != 0) return;
  TargetFollowCamera_setActive(P(self, 0x88), 0);
  void* e = operator_new_(0x68);
  Explosion_ctor(e, 0);
  int pl = I(self, 0);
  I(self, 0x8c) = (int)e;
  Player_setActive_(pl);
  void* o = g_explode_obj;
  void (*fn)(void*, int) = g_explode_fn;
  fn(*(void**)o, *(int*)(*(void**)o));
  fn(*(void**)o, I(self, 0x1c));
  fn(*(void**)o, 0x1b);
  fn(*(void**)o, 0x23);
  fn(*(void**)o, 0x8d5);
  fn(*(void**)o, 0x8d4);
  fn(*(void**)o, 0x8cc);
  fn(*(void**)o, 0x447);
  fn(*(void**)o, 0x448);
  fn(*(void**)o, 0x449);
  return PlayerEgo_explode_ext(self, 0);
}
