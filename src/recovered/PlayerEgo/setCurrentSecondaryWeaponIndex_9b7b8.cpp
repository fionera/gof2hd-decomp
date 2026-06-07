#include "class.h"
extern PlayerEgo* g_PlayerEgo_singleton;
void PlayerEgo_setCurrentSecondaryWeaponIndex(PlayerEgo* self, int idx) {
  I(self, 0x10c) = idx;
  I(g_PlayerEgo_singleton, 0xf4) = idx;
}
