#include "class.h"
extern void* g_engine_status;
extern "C" void* Status_getShip(void*);
extern "C" int Ship_getFirstEquipmentOfSort(void*, int);
extern "C" int Status_inAlienOrbit(void*);
extern "C" void* Status_getStation(void*);
extern "C" int Station_getIndex(void*);
extern "C" int Status_getCurrentCampaignMission(void*);
extern "C" float Status_getGammaRayDamagePerSecond(void*, int, int);
extern void* g_engine_fmod;
extern "C" void FModSound_play(void*, void*, int, float, int);
extern "C" void PlayerEgo_StopEngineSound_ext(void*);
void PlayerEgo_StopEngineSound(PlayerEgo* self) {
  if (C(self, 0x356) == 0 || I(self, 0x1c4) != 1) {
    if (Ship_getFirstEquipmentOfSort(Status_getShip(g_engine_status), 0x26) != 0
        && Status_inAlienOrbit(g_engine_status) == 0) {
      void* obj = g_engine_status;
      int idx = Station_getIndex(Status_getStation(obj));
      int cm = Status_getCurrentCampaignMission(g_engine_status);
      float g = Status_getGammaRayDamagePerSecond(obj, idx, cm);
      if (0.0f < g && I(self, 0xb4) != -1) {
        FModSound_play(*(void**)g_engine_fmod, P(self, 0xb4), 0, g, 0);
      }
    }
  }
  return PlayerEgo_StopEngineSound_ext(P(self, 0));
}
