#include "class.h"

struct PlayerEgo;
struct Level;
struct LevelScript;
struct Layout;

extern "C" void PaintCanvas_Begin2d();                                   // 0x71d34
extern "C" void Level_getStarSystem(Level *l);                          // 0x727c0
extern "C" void StarSystem_render2D(int ss);                            // 0x74458
extern "C" int LevelScript_startSequenceOver();                         // (startSequenceOver)
extern "C" int LevelScript_startSequence(LevelScript *s);
extern "C" void MGame_drawRadio(MGame *self);                           // Radio::draw wrapper
extern "C" void StarMap_draw(int map);
extern "C" void Layout_drawHelpWindow(Layout *l);
extern "C" void Level_render2D(int level);
extern "C" void Hud_drawPauseButton(void *hud);
extern "C" void *Status_getCampaignMission();
extern "C" int Mission_getType(void *m);                                // 0x72874
extern "C" int LevelScript_getEvent(LevelScript *s);
extern "C" void Hud_drawOrbitInformation(void *hud);
extern "C" void DialogueWindow_draw(int w);
extern "C" int Status_getCurrentCampaignMission();                      // 0x71770
extern "C" int PlayerEgo_isDockingToPlanet(PlayerEgo *p);
extern "C" void PlayerEgo_draw(PlayerEgo *p);
extern "C" int PlayerEgo_isMining(PlayerEgo *p);                        // 0x77adc
extern "C" int PlayerEgo_isHacking(PlayerEgo *p);
extern "C" void MGame_drawRadar(MGame *self);                          // Radar::draw wrapper
extern "C" void MGame_nextCamId(MGame *self);                          // 0x... nextCamId
extern "C" void MGame_drawHud(MGame *self);                            // Hud::draw wrapper
extern "C" void Radar_drawCurrentLock(void *radar);
extern "C" void Layout_drawMissionRewardMessage(int layout);
extern "C" void ChoiceWindow_draw(int w);
extern "C" void Hud_drawMenu(int hud);
extern "C" void PaintCanvas_SetColor(unsigned color);                  // 0x6fac0
extern "C" void PaintCanvas_DrawImage2D5(int pc, int img, int x, int y, int anchor); // 0x71d70
extern "C" void Layout_drawFade(Layout *l);                           // 0x... drawFade
extern "C" void MenuTouchWindow_draw(void *w);                         // 0x755d4
extern "C" void Vector_assign(void *dst, void *src);                  // 0x6eb3c
extern "C" int ApplicationManager_GetEngine();                        // 0x7258c
extern "C" void PaintCanvas_End2d();                                  // 0x71d40? End2d
extern "C" void MGame_drawFadeMessage(MGame *self, int pc);           // splash/fade text helper

__attribute__((visibility("hidden"))) extern int *g_r2dGuard;    // @0x1908d4 (stack guard [0])
__attribute__((visibility("hidden"))) extern unsigned **g_r2dCanvas; // @0x1908e8
__attribute__((visibility("hidden"))) extern Layout ***g_r2dHelpLayout; // @0x190962
__attribute__((visibility("hidden"))) extern int **g_r2dPauseFlag; // @0x1909cc
__attribute__((visibility("hidden"))) extern int **g_r2dRewardLayout; // @0x190d4a
__attribute__((visibility("hidden"))) extern Layout ***g_r2dFadeLayout; // @0x190a76
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

// MGame::OnRender2D(): draw the 2D HUD / menus / overlays for the active state.
extern "C" void MGame_OnRender2D(MGame *self) {
    int *guard = g_r2dGuard;
    int g0 = *guard;
    if (F<uint8_t>(self, 0x54) == 0) {
        if (*guard != g0) __stack_chk_fail();
        return;
    }

    PaintCanvas_Begin2d();

    if (F<uint8_t>(self, 0x5d) != 0 && F<uint8_t>(self, 0xc9) != 0) {
        // Menu-touch-window / star-system background path.
        bool drawSS = true;
        if (F<uint8_t>(self, 0x15e) != 0 && F<uint8_t>(self, 0x15c) != 0 &&
            *(uint8_t *)((char *)F<int>(self, 0x88) + 1) == 0) {
            drawSS = true;
        } else {
            MenuTouchWindow_draw(F<void *>(self, 0x88));
            drawSS = (F<uint8_t>(self, 0x15e) != 0);
        }
        if (drawSS) {
            Level_getStarSystem(F<Level *>(self, 0x78));
            StarSystem_render2D(0);
        }
        float v[4]; *(int *)&v[0] = 0x3f000000; *(int *)&v[1] = 0x3f000000; v[2] = 0;
        int eng = ApplicationManager_GetEngine();
        Vector_assign((char *)eng + 0x3cc, v);
        PaintCanvas_End2d();
        if (*guard != g0) __stack_chk_fail();
        return;
    }

    if (F<uint8_t>(self, 0x5d) == 0 || F<uint8_t>(self, 0x111) == 0) {
        // World/HUD render path.
        if (F<uint8_t>(self, 0x111) == 0) {
            // Cinematic-sequence path (field 0x49 etc.).
            Level_getStarSystem(F<Level *>(self, 0x78));
            StarSystem_render2D(0);
            if (LevelScript_startSequenceOver() != 0 ||
                LevelScript_startSequence(F<LevelScript *>(self, 0x7c)) == 0)
                MGame_drawRadio(self);
        } else if (F<uint8_t>(self, 0xc7) != 0) {
            StarMap_draw(F<int>(self, 0x90));
            Layout *hl = **g_r2dHelpLayout;
            if (*(uint8_t *)hl != 0)
                Layout_drawHelpWindow(hl);
        } else {
            Level_render2D(F<int>(self, 0x78));
            if (**g_r2dPauseFlag == 0)
                Hud_drawPauseButton(F<void *>(self, 0x74));

            if (Mission_getType(Status_getCampaignMission()) == 0xaa) {
                if (LevelScript_getEvent(F<LevelScript *>(self, 0x7c)) == 0)
                    Hud_drawOrbitInformation(F<void *>(self, 0x74));
                MGame_drawRadio(self);
                if (F<uint8_t>(self, 0x5e) != 0)
                    DialogueWindow_draw(F<int>(self, 0x8c));
                F<uint8_t>(self, 0x111) = 0;
                F<uint8_t>(self, 0x5d) = 0;
            } else if (F<uint8_t>(self, 0x5f) != 0 || F<uint8_t>(self, 0x15d) != 0) {
                int cm = Status_getCurrentCampaignMission();
                if (cm > 7 && F<uint8_t>(self, 0x80) == 0 && F<uint8_t>(self, 0x15d) == 0 &&
                    PlayerEgo_isDockingToPlanet(F<PlayerEgo *>(self, 0x58)) == 0 &&
                    LevelScript_getEvent(F<LevelScript *>(self, 0x7c)) == 0)
                    Hud_drawOrbitInformation(F<void *>(self, 0x74));
                if (LevelScript_startSequenceOver() != 0 ||
                    LevelScript_startSequence(F<LevelScript *>(self, 0x7c)) == 0)
                    MGame_drawRadio(self);
                if (F<uint8_t>(self, 0x5e) != 0)
                    DialogueWindow_draw(F<int>(self, 0x8c));
                F<uint8_t>(self, 0x111) = 0;
                F<uint8_t>(self, 0x5d) = 0;
            } else if (F<uint8_t>(self, 0x60) == 0) {
                PlayerEgo_draw(F<PlayerEgo *>(self, 0x58));
                if (PlayerEgo_isMining(F<PlayerEgo *>(self, 0x58)) == 0 &&
                    F<uint8_t>(self, 0xc7) == 0 &&
                    (PlayerEgo_isHacking(F<PlayerEgo *>(self, 0x58)) == 0 ||
                     F<uint8_t>(self, 0xc9) != 0))
                    MGame_drawRadar(self);
                if (F<uint8_t>(self, 0x5e) == 0) {
                    MGame_nextCamId(self);
                    MGame_drawHud(self);
                    MGame_drawRadio(self);
                    Radar_drawCurrentLock(F<void *>(self, 0x80));
                    Layout_drawMissionRewardMessage(**g_r2dRewardLayout);
                } else {
                    DialogueWindow_draw(F<int>(self, 0x8c));
                }
                if (F<uint8_t>(self, 0xc5) != 0 || F<uint8_t>(self, 0xc6) != 0 ||
                    F<uint8_t>(self, 0xce) != 0 || F<uint8_t>(self, 0xc4) != 0)
                    ChoiceWindow_draw(F<int>(self, 0x94));
                if (F<uint8_t>(self, 0xca) != 0)
                    Hud_drawMenu(F<int>(self, 0x74));
            } else if (!(F<int>(self, 0x4c) < (int)(F<unsigned>(self, 0x48) < 0xbb9))) {
                // Loading/jump splash text.
                PaintCanvas_SetColor(F<unsigned>(self, 0x4));
                PaintCanvas_DrawImage2D5(*(int *)g_r2dCanvas, F<int>(self, 0x10), 0, 0, 'D');
                if (F<int>(self, 0x50) >= 4000)
                    MGame_drawFadeMessage(self, *(int *)g_r2dCanvas);
            }
            Layout_drawFade(**g_r2dFadeLayout);
        }
    }

    PaintCanvas_End2d();
    if (*guard != g0) __stack_chk_fail();
}
