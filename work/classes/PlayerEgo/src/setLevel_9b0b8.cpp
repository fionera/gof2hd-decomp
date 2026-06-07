#include "class.h"
extern "C" void* AEGeometry_getMatrix2(void*);
extern "C" void* ParticleSystemManager_addSystem(void*, void*, int, int);
extern "C" void ParticleSystemManager_enableSystemEmit3(void*, int, int);
extern void* g_setLevel_status;
extern "C" int Status_getCurrentCampaignMission(void*);
extern "C" void PlayerEgo_setLevel_ext(void*, int, int);
void PlayerEgo_setLevel(PlayerEgo* self, void* level) {
  I(self, 0xc) = (int)level;
  void* src = P(level, 0x74);
  void* gm = AEGeometry_getMatrix2(P(self, 8));
  void* sys = ParticleSystemManager_addSystem(src, gm, 9, 0);
  I(self, 0x2fc) = (int)sys;
  ParticleSystemManager_enableSystemEmit3(P(P(self, 0xc), 0x74), (int)sys, 0);
  if (Status_getCurrentCampaignMission(g_setLevel_status) > 1) return;
  void* src2 = P(P(self, 0xc), 0x78);
  void* gm2 = AEGeometry_getMatrix2(P(self, 8));
  void* sys2 = ParticleSystemManager_addSystem(src2, gm2, 0xf, 0);
  I(self, 0x300) = (int)sys2;
  ParticleSystemManager_enableSystemEmit3(P(P(self, 0xc), 0x78), (int)sys2, 0);
  void* src3 = P(P(self, 0xc), 0x84);
  void* gm3 = AEGeometry_getMatrix2(P(self, 8));
  void* sys3 = ParticleSystemManager_addSystem(src3, gm3, 0x2a, 0);
  I(self, 0x304) = (int)sys3;
  return PlayerEgo_setLevel_ext(P(P(self, 0xc), 0x84), (int)sys3, 0);
}
