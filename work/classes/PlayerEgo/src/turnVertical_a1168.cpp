#include "class.h"
extern "C" void PlayerEgo_turnVertical_neg();
extern "C" void PlayerEgo_turnVertical_pos(PlayerEgo*);
void PlayerEgo_turnVertical(PlayerEgo* self, int a, float v) {
  if (v < -0.0f) { PlayerEgo_turnVertical_neg(); return; }
  if (v > 0.0f) { PlayerEgo_turnVertical_pos(self); return; }
}
