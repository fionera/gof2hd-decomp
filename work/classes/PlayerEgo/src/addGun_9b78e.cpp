#include "class.h"
extern "C" void Player_addGun2(void*, void*, int);
extern "C" void PlayerEgo_addGun2_ext(PlayerEgo*);
void PlayerEgo_addGun2(PlayerEgo* self, void* arr, int x) {
  Player_addGun2(P(self, 0), arr, x);
  return PlayerEgo_addGun2_ext(self);
}
