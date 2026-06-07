#include "class.h"
extern void* g_mining_status;
extern "C" void* Status_getMission(void*);
extern "C" int Mission_isEmpty(void*);
extern "C" int Status_inAlienOrbit(void*);
extern "C" void* Status_getStation(void*);
extern "C" int Station_getIndex(void*);
bool PlayerEgo_isDockedToMiningPlant(PlayerEgo* self) {
  if (C(self, 0x356) != 0 && I(self, 0x1c4) == 1) {
    if (Mission_isEmpty(Status_getMission(g_mining_status)) != 0
        && Status_inAlienOrbit(g_mining_status) == 0) {
      return Station_getIndex(Status_getStation(g_mining_status)) == 0x67;
    }
  }
  return false;
}
