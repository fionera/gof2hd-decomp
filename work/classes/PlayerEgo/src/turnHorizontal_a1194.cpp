#include "class.h"
extern "C" void PlayerEgo_turnHorizontal_neg(PlayerEgo*);
extern "C" void PlayerEgo_turnHorizontal_pos(PlayerEgo*);
void PlayerEgo_turnHorizontal(PlayerEgo* self, int a, float v) {
  if (v < 0.0f) { PlayerEgo_turnHorizontal_neg(self); return; }
  if (v > 0.0f) { PlayerEgo_turnHorizontal_pos(self); return; }
}
