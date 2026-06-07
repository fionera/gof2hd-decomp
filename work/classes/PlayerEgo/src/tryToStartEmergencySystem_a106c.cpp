#include "class.h"
extern "C" int Player_getHitpoints(void*);
extern "C" void Player_setHitpoints(void*, int);
extern "C" void Player_setVulnerable(void*, int);
extern void* g_emerg_status;
extern "C" void* Status_getShip(void*);
extern "C" int Ship_getFirstEquipmentOfSort(void*, int);
extern "C" float Ship_removeEquipment(void*, int);
extern void* g_emerg_fmod;
extern "C" void FModSound_play(void*, void*, int, float, int);
int PlayerEgo_tryToStartEmergencySystem(PlayerEgo* self) {
  if (I(self, 0xac) == 0 || I(self, 0x30c) != 0) return 0;
  if (Player_getHitpoints(P(self, 0)) > 1) return 0;
  Player_setHitpoints(P(self, 0), 1);
  I(self, 0x30c) = I(self, 0x310);
  Player_setVulnerable(P(self, 0), 0);
  void* s1 = Status_getShip(g_emerg_status);
  int eq = Ship_getFirstEquipmentOfSort(Status_getShip(g_emerg_status), 0x1b);
  float f = Ship_removeEquipment(s1, eq);
  FModSound_play(*(void**)g_emerg_fmod, (void*)0x45b, 0, f, 0);
  return 1;
}
