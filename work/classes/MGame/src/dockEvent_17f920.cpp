#include "class.h"

struct PlayerEgo;
struct Player;
struct Station;
struct StarMap;
struct Engine;
struct ChoiceWindow;
struct Mission;
struct Level;

extern "C" void PlayerEgo_getPosition(void *out, PlayerEgo *p);          // 0x7264c
extern "C" uint8_t Level_collideStream(int level, int x, int y, int z);  // 0x78604
extern "C" uint8_t Level_collideStation(int level, int x, int y, int z); // 0x78610
extern "C" Mission *Status_getMission();                                 // 0x72850
extern "C" int Mission_isEmpty(Mission *m);                              // 0x72868
extern "C" int Mission_getType(Mission *m);                              // 0x72874
extern "C" int PlayerEgo_isAutoPilot(PlayerEgo *p);                      // (isAutoPilot)
extern "C" void Hud_hudEvent(int hud, int ev, int player);              // (Hud::hudEvent)
extern "C" int PlayerEgo_goingToStream(PlayerEgo *p);                    // 0x... goingToStream
extern "C" int PlayerEgo_isDockingToStream(PlayerEgo *p);
extern "C" int PlayerEgo_isDockedToStream(PlayerEgo *p);
extern "C" void PlayerEgo_dockToStream(PlayerEgo *p, int a);
extern "C" int Player_getHitpoints(Player *p);                           // 0x724f0
extern "C" int Player_getShieldHP(Player *p);                            // 0x726c4
extern "C" int Player_getArmorHP(Player *p);                             // 0x72928
extern "C" int Player_getGammaHP(Player *p);                             // 0x72814
extern "C" int PlayerEgo_getCurrentSecondaryWeaponIndex(PlayerEgo *p);   // 0x76d20
extern "C" void *operator_new(unsigned sz);                              // 0x6eb24
extern "C" void StarMap_ctor(StarMap *m, int a, Mission *b, int c, int d); // 0x75424
extern "C" int ApplicationManager_GetEngine();                           // 0x7258c
extern "C" void Engine_SetPostEffect(Engine *e, int id, int on);         // 0x707f8
extern "C" void StarMap_initLights(StarMap *m);                          // 0x78298
extern "C" void StarMap_setJumpMapMode(StarMap *m, int a, int b);        // 0x75430
extern "C" void PlayerEgo_setAutoPilot(PlayerEgo *p, int v);             // 0x728d4
extern "C" void MGame_pauseSounds(MGame *self);                          // 0x7822c
extern "C" void ChoiceWindow_ctor(ChoiceWindow *w);                      // 0x74584
extern "C" int PlayerEgo_getAutoPilotTarget(PlayerEgo *p);
extern "C" int Level_getLandmarks(Level *l);                             // 0x725b0
extern "C" int PlayerEgo_collidesWithStation(PlayerEgo *p);
extern "C" int Status_getCurrentCampaignMission();                       // 0x71770
extern "C" Station *Status_getStation();                                 // 0x71c14
extern "C" int Station_getIndex(Station *s);                             // 0x71824
extern "C" int PlayerEgo_goingToStation(PlayerEgo *p);
extern "C" int Status_inAlienOrbit();                                    // 0x723d0
extern "C" int Status_inEmptyOrbit(Station *s);
extern "C" void Achievements_checkForNewMedal(void *a, PlayerEgo *p);
extern "C" void ApplicationManager_SetCurrentApplicationModule(unsigned m); // 0x71d64
// Builds and sets the "dock at <station>?" choice text, then ChoiceWindow::left().
extern "C" void MGame_buildDockChoice(MGame *self, int textId, int prefixLit, int suffixLit);

__attribute__((visibility("hidden"))) extern int *g_deGuard;     // @0x18f932 (stack guard [0])
__attribute__((visibility("hidden"))) extern int *g_deStatus;    // @0x18f970 (piVar6)
__attribute__((visibility("hidden"))) extern int *g_deAutoFlag;  // @0x18fa3a
__attribute__((visibility("hidden"))) extern int g_dePostEffect; // @0x18fe0c
__attribute__((visibility("hidden"))) extern int g_deTextA;      // @0x18fa82
__attribute__((visibility("hidden"))) extern int g_deLitA0;      // @0x18fa9a
__attribute__((visibility("hidden"))) extern int g_deLitA1;      // @0x18fac0
__attribute__((visibility("hidden"))) extern int g_deTextB;      // @0x18fc74
__attribute__((visibility("hidden"))) extern int g_deLitB0;      // @0x18fc8c
__attribute__((visibility("hidden"))) extern int g_deLitB1;      // @0x18fcb2
__attribute__((visibility("hidden"))) extern int **g_deAchieve;  // @0x18fb90
__attribute__((visibility("hidden"))) extern int **g_deAlienFlag;// @0x18fb9c
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

static void savePlayerStats(MGame *self, int *status) {
    PlayerEgo *ego = F<PlayerEgo *>(self, 0x58);
    Player *pl = *(Player **)((char *)ego);
    *(int *)((char *)status + 100) = Player_getHitpoints(pl);
    *(int *)((char *)status + 0x5c) = Player_getShieldHP(pl);
    *(int *)((char *)status + 0x60) = Player_getArmorHP(pl);
    *(int *)((char *)status + 0x68) = Player_getGammaHP(pl);
}

// MGame::dockEvent(): handle proximity to a jumpgate/station while flying.
extern "C" void MGame_dockEvent(MGame *self) {
    int *guard = g_deGuard;
    int g0 = *guard;

    float pos[4];
    PlayerEgo_getPosition(pos, F<PlayerEgo *>(self, 0x58));
    F<uint8_t>(self, 0xcb) = Level_collideStream(F<int>(self, 0x78),
                                                 (int)pos[0], (int)pos[1], (int)pos[2]);
    PlayerEgo_getPosition(pos, F<PlayerEgo *>(self, 0x58));
    F<uint8_t>(self, 0xcc) = Level_collideStation(F<int>(self, 0x78),
                                                  (int)pos[0], (int)pos[1], (int)pos[2]);

    int *status = g_deStatus;
    Mission *m = Status_getMission();
    bool special = Mission_isEmpty(m) != 0 ||
                   Mission_getType(Status_getMission()) == 0xb ||
                   Mission_getType(Status_getMission()) == 0 ||
                   Mission_getType(Status_getMission()) == 0xbd ||
                   Mission_getType(Status_getMission()) == 0xab ||
                   Mission_getType(Status_getMission()) == 0xac;

    if (!special) {
        if ((F<uint8_t>(self, 0xcc) != 0 || F<uint8_t>(self, 0xcb) != 0) &&
            PlayerEgo_isAutoPilot(F<PlayerEgo *>(self, 0x58)) != 0) {
            Hud_hudEvent(F<int>(self, 0x74), 0x15, F<int>(self, 0x58));
        }
        if (*guard != g0) __stack_chk_fail();
        return;
    }

    // special-mission docking path.
    if (F<uint8_t>(self, 0xcb) != 0) {
        if (PlayerEgo_goingToStream(F<PlayerEgo *>(self, 0x58)) != 0 &&
            PlayerEgo_isDockingToStream(F<PlayerEgo *>(self, 0x58)) == 0 &&
            PlayerEgo_isDockedToStream(F<PlayerEgo *>(self, 0x58)) == 0) {
            PlayerEgo_dockToStream(F<PlayerEgo *>(self, 0x58), 0);
            F<uint8_t>(self, 0x15c) = 0;
            F<uint8_t>(self, 0x111) = 1;
            if (*guard != g0) __stack_chk_fail();
            return;
        }
        if (F<uint8_t>(self, 0xcb) != 0) {
            savePlayerStats(self, status);
            *(int *)((char *)status + 0xf4) = PlayerEgo_getCurrentSecondaryWeaponIndex(F<PlayerEgo *>(self, 0x58));
            int autop = PlayerEgo_isAutoPilot(F<PlayerEgo *>(self, 0x58));
            int *autoFlag = g_deAutoFlag;
            if (*autoFlag == 0 || autop == 0) {
                if (F<uint8_t>(self, 0xc7) != 0) {
                    F<int>(self, 0x30) = 0;
                    F<int>(self, 0x34) = 0;
                    if (*guard != g0) __stack_chk_fail();
                    return;
                }
                PlayerEgo_isAutoPilot(F<PlayerEgo *>(self, 0x58));
                if (PlayerEgo_goingToStream(F<PlayerEgo *>(self, 0x58)) == 0) {
                    if (*guard != g0) __stack_chk_fail();
                    return;
                }
                if (*autoFlag == 0) {
                    F<uint8_t>(self, 0x15c) = 0;
                    F<uint8_t>(self, 0x111) = 1;
                    if (F<int>(self, 0x90) == 0) {
                        StarMap *sm = (StarMap *)operator_new(0x1e8);
                        StarMap_ctor(sm, 0, 0, 0, -1);
                        F<StarMap *>(self, 0x90) = sm;
                    }
                    Engine *eng = (Engine *)ApplicationManager_GetEngine();
                    Engine_SetPostEffect(eng, g_dePostEffect, 0);
                    StarMap_initLights(F<StarMap *>(self, 0x90));
                    StarMap_setJumpMapMode(F<StarMap *>(self, 0x90), 1, 0);
                    PlayerEgo_setAutoPilot(F<PlayerEgo *>(self, 0x58), 0);
                    F<uint8_t>(self, 0x5d) = 1;
                    F<uint8_t>(self, 0xc7) = 1;
                    MGame_pauseSounds(self);
                    F<uint8_t>(self, 0x111) = 1;
                    if (*guard != g0) __stack_chk_fail();
                    return;
                }
                if (F<uint8_t>(self, 0xc5) != 0) { if (*guard != g0) __stack_chk_fail(); return; }
                F<uint8_t>(self, 0x15c) = 0;
                ChoiceWindow *cw = F<ChoiceWindow *>(self, 0x94);
                F<uint8_t>(self, 0x111) = 1;
                if (cw == 0) {
                    cw = (ChoiceWindow *)operator_new(0x5c);
                    ChoiceWindow_ctor(cw);
                    F<ChoiceWindow *>(self, 0x94) = cw;
                }
                MGame_buildDockChoice(self, g_deTextB, g_deLitB0, g_deLitB1);
            } else {
                if (F<uint8_t>(self, 0xc5) != 0) { if (*guard != g0) __stack_chk_fail(); return; }
                F<uint8_t>(self, 0x15c) = 0;
                ChoiceWindow *cw = F<ChoiceWindow *>(self, 0x94);
                F<uint8_t>(self, 0x111) = 1;
                if (cw == 0) {
                    cw = (ChoiceWindow *)operator_new(0x5c);
                    ChoiceWindow_ctor(cw);
                    F<ChoiceWindow *>(self, 0x94) = cw;
                }
                MGame_buildDockChoice(self, g_deTextA, g_deLitA0, g_deLitA1);
            }
            F<uint8_t>(self, 0x5d) = 1;
            F<uint8_t>(self, 0xc5) = 1;
            MGame_pauseSounds(self);
            PlayerEgo_setAutoPilot(F<PlayerEgo *>(self, 0x58), 0);
            if (*guard != g0) __stack_chk_fail();
            return;
        }
    }

    if (F<uint8_t>(self, 0xcc) == 0) {
        int tgt = PlayerEgo_getAutoPilotTarget(F<PlayerEgo *>(self, 0x58));
        int lm = Level_getLandmarks(F<Level *>(self, 0x78));
        if (tgt != *(int *)((char *)lm + 4) ||
            PlayerEgo_collidesWithStation(F<PlayerEgo *>(self, 0x58)) == 0) {
            if (*guard != g0) __stack_chk_fail();
            return;
        }
    }

    int cm = Status_getCurrentCampaignMission();
    if (cm > 0x30 && Status_getCurrentCampaignMission() < 0x37) {
        if (Station_getIndex(Status_getStation()) != 0x4a) {
            Hud_hudEvent(F<int>(self, 0x74), 0x15, F<int>(self, 0x58));
            if (*guard != g0) __stack_chk_fail();
            return;
        }
    }
    if (PlayerEgo_goingToStation(F<PlayerEgo *>(self, 0x58)) != 0 &&
        Status_inAlienOrbit() == 0 &&
        Status_inEmptyOrbit((Station *)*status) == 0) {
        Achievements_checkForNewMedal(*g_deAchieve, F<PlayerEgo *>(self, 0x58));
        **g_deAlienFlag = 0;
        savePlayerStats(self, status);
        ApplicationManager_SetCurrentApplicationModule(F<unsigned>(self, 0x8));
        F<uint8_t>(self, 0x54) = 0;
    }

    if (*guard != g0) __stack_chk_fail();
}
