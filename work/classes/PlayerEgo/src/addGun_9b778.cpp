#include "class.h"
extern "C" void Player_addGun(void*, void*, int);
extern "C" void PlayerEgo_addGun_ext(PlayerEgo*);
void PlayerEgo_addGun(PlayerEgo* self, void* gun, int x) {
  Player_addGun(P(self, 0), gun, x);
  return PlayerEgo_addGun_ext(self);
}
