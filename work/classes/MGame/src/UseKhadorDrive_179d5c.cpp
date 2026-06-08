#include "class.h"

struct PlayerEgo;
struct Mission;
struct Station;
struct StarMap;
struct Engine;

extern "C" int PlayerEgo_isChargingDrive(PlayerEgo *p);            // 0x77a7c
extern "C" Mission *Status_getMission();                          // 0x72850
extern "C" int Mission_isEmpty(Mission *m);                       // 0x72868
extern "C" int Status_getCurrentCampaignMission();                // 0x71770
extern "C" int Mission_getType(Mission *m);                       // 0x72874
extern "C" int Status_inAlienOrbit();                             // 0x723d0
extern "C" Station *Status_getStation();                          // 0x71c14
extern "C" int Station_getIndex(Station *s);                      // 0x71824
extern "C" Mission *Status_getCampaignMission();                  // 0x7285c
extern "C" int Mission_getTargetStation(Mission *m);              // 0x73738
extern "C" int Level_getPlayer(MGame *self);                      // 0x72034
extern "C" void PlayerEgo_resetGunDelay(PlayerEgo *p);            // 0x72ca0
extern "C" void MGame_startChargingJumpDrive(MGame *self);        // 0x78280
extern "C" void MGame_resumeSounds(MGame *self);                  // 0x7828c
extern "C" void Hud_closeHudMenu(void *hud);                      // 0x77a04
extern "C" int PlayerEgo_hasVolatileGoods(PlayerEgo *p);          // 0x77b24
extern "C" void *GameText_getText(int id);                        // 0x72f70
extern "C" void ChoiceWindow_set(void *w, void *txt);             // 0x74794
extern "C" void MGame_pauseSounds(MGame *self);                   // 0x7822c
extern "C" void *operator_new(unsigned sz);                       // 0x6eb24
extern "C" void StarMap_ctor(StarMap *m, int a, Mission *b, int c, int d); // 0x75424
extern "C" int ApplicationManager_GetEngine();                    // 0x7258c
extern "C" void Engine_SetPostEffect(Engine *e, int id, int on);  // 0x707f8
extern "C" void StarMap_initLights(StarMap *m);                   // 0x78298
extern "C" void StarMap_setJumpMapMode(StarMap *m, int a, int b); // 0x75430
extern "C" void StarMap_askForJumpIntoAlienWorld(StarMap *m);     // 0x782a4
extern "C" int Galaxy_getStation(int idx);                        // 0x71c44
// Tail helpers.
extern "C" void Hud_hudEvent(void *hud, int ev, int player, int arg); // @0x1ac7a8
extern "C" void MGame_jumpFinish(int *globals, int one);          // @0x1ac808
extern "C" void MGame_starMapShown(int hud);                      // @0x1ac818

__attribute__((visibility("hidden"))) extern int *g_kdStatus;     // @0x189d7e (*piVar6)
__attribute__((visibility("hidden"))) extern int **g_kdJumpDst;   // @0x189e36
__attribute__((visibility("hidden"))) extern int **g_kdVolText;   // @0x189ed2
__attribute__((visibility("hidden"))) extern int **g_kdGalaxy;    // @0x189e8e
__attribute__((visibility("hidden"))) extern int **g_kdAlienDst;  // @0x189e9a
__attribute__((visibility("hidden"))) extern int g_kdPostEffect;  // @0x189f98 (DAT)

// MGame::UseKhadorDrive(): trigger a hyperspace jump or open the jump star map.
extern "C" void MGame_UseKhadorDrive(MGame *self) {
    if (PlayerEgo_isChargingDrive(F<PlayerEgo *>(self, 0x58)) != 0) return;

    int *status = g_kdStatus;
    Mission *m = Status_getMission();
    bool special =
        Mission_isEmpty(m) != 0 ||
        Status_getCurrentCampaignMission() == 0x4e ||
        Mission_getType(m) == 0xb ||
        Mission_getType(m) == 0 ||
        Mission_getType(m) == 0xbd ||
        Mission_getType(m) == 0xd ||
        Mission_getType(m) == 0xab ||
        Mission_getType(m) == 0xac;

    if (!special) {
        if (Status_getCurrentCampaignMission() == 0x41 && Status_inAlienOrbit() == 0) {
            int idx = Station_getIndex(Status_getStation());
            Status_getCampaignMission();
            if (idx == Mission_getTargetStation(Status_getCampaignMission()))
                special = true;
        }
    }

    if (special) {
        int player = Level_getPlayer(self);
        return Hud_hudEvent(F<void *>(self, 0x74), 0x15, player, 0);
    }

    PlayerEgo_resetGunDelay(F<PlayerEgo *>(self, 0x58));
    if (Status_getCurrentCampaignMission() == 0x4e) {
        **g_kdJumpDst = *(int *)((char *)*status + 0x78);
        F<uint8_t>(self, 0xdd) = 1;
        MGame_startChargingJumpDrive(self);
        F<uint8_t>(self, 0x5d) = 0;
        MGame_resumeSounds(self);
        F<uint8_t>(self, 0xd6) = 0;
        Hud_closeHudMenu(F<void *>(self, 0x74));
        return MGame_jumpFinish(status, 1);
    }

    if (Status_inAlienOrbit() == 0) {
        if (PlayerEgo_hasVolatileGoods(F<PlayerEgo *>(self, 0x58)) != 0) {
            void *txt = GameText_getText(**g_kdVolText);
            ChoiceWindow_set(F<void *>(self, 0x94), txt);
            F<uint8_t>(self, 0x5d) = 1;
            F<uint8_t>(self, 0xce) = 1;
            F<uint8_t>(self, 0x111) = 1;
            F<uint8_t>(self, 0xd6) = 0;
            Hud_closeHudMenu(F<void *>(self, 0x74));
            MGame_pauseSounds(self);
            F<uint8_t>(self, 0x17c) = 0;
            return;
        }
        if (F<int>(self, 0x90) == 0) {
            StarMap *sm = (StarMap *)operator_new(0x1e8);
            StarMap_ctor(sm, 0, 0, 0, -1);
            F<StarMap *>(self, 0x90) = sm;
        }
        Engine *eng = (Engine *)ApplicationManager_GetEngine();
        Engine_SetPostEffect(eng, g_kdPostEffect, 0);
        StarMap_initLights(F<StarMap *>(self, 0x90));
        F<uint8_t>(self, 0xdd) = 1;
        StarMap_setJumpMapMode(F<StarMap *>(self, 0x90), 1, 1);
        if (Status_inAlienOrbit() == 0)
            StarMap_askForJumpIntoAlienWorld(F<StarMap *>(self, 0x90));
        F<uint8_t>(self, 0x5d) = 1;
        F<uint8_t>(self, 0xc7) = 1;
        MGame_pauseSounds(self);
        F<uint8_t>(self, 0xd6) = 0;
        return MGame_starMapShown(F<int>(self, 0x74));
    }

    // In alien orbit.
    if (Status_getCurrentCampaignMission() == 0x50)
        *(int *)((char *)*status + 0x84) = 100;
    int station = Galaxy_getStation(**g_kdGalaxy);
    **g_kdAlienDst = station;
    F<uint8_t>(self, 0xdd) = 1;
    MGame_startChargingJumpDrive(self);
    F<uint8_t>(self, 0x5d) = 0;
    MGame_resumeSounds(self);
    F<uint8_t>(self, 0xd6) = 0;
    return MGame_starMapShown(F<int>(self, 0x74));
}
