#include "class.h"

struct PlayerEgo;
struct Layout;

extern "C" int Status_getCurrentCampaignMission();                       // 0x71770
extern "C" void GameRecord_load(void *rec);
extern "C" void Globals_playMusicAndFadeOutCurrent(int track);
extern "C" void ApplicationManager_SetCurrentApplicationModule(unsigned m); // 0x71d64
extern "C" int Layout_OnTouchBegin(int layout, int p1);                 // 0x... (Layout::OnTouchBegin)
extern "C" uint8_t StarMap_OnTouchBegin(int map, int p1);
extern "C" void ChoiceWindow_OnTouchBegin(int w, int p1);
extern "C" void DialogueWindow_OnTouchBegin(int w, int p1);
extern "C" int ApplicationManager_GetApplicationData();                 // 0x71704
extern "C" void MenuTouchWindow_OnTouchBegin(int w, int p1, void *p2);
extern "C" int MenuTouchWindow_isShowingMessage(void *w);               // 0x781e4
extern "C" int MenuTouchWindow_isMakingScreenshot(void *w);             // 0x781f0
extern "C" int Hud_touchBegin(void *hud, int p1, int p2, void *p3);
// Module-switch tail helper @0x1ab908 (replay/menu transition).
extern "C" void MGame_endRunModule(int field8, int code);
// The large post-Hud-touch reaction body (time-extender, dock dialogue, free-cam
// init, etc.) is one logical unit; kept as a helper to remain faithful + compiling.
extern "C" void MGame_handleHudTouchAction(MGame *self, int p1, int p2, void *p3, unsigned hudResult);

__attribute__((visibility("hidden"))) extern Layout ***g_tbStarLayout; // @0x188e90
__attribute__((visibility("hidden"))) extern int **g_tbRecordTrack;    // @0x188f1a
__attribute__((visibility("hidden"))) extern int **g_tbMenuTrack;      // @0x188f3e

// MGame::OnTouchBegin(int p1, int p2, void *touchId)
extern "C" void MGame_OnTouchBegin(MGame *self, int p1, int p2, void *touchId) {
    // Track the active touch id (field 0xc0) when none is held.
    if (F<int>(self, 0xc0) == 0)
        F<void *>(self, 0xc0) = touchId;

    if (F<uint8_t>(self, 0x5d) == 0) {
        // Game-over / replay splash tap dispatch.
        if (F<uint8_t>(self, 0x60) != 0 && F<int>(self, 0x50) >= 4000) {
            int cm = Status_getCurrentCampaignMission();
            if (cm == 0x9e) {
                F<uint8_t>(self, 0x54) = 0;
                return MGame_endRunModule(F<int>(self, 0x8), 2);
            }
            if (F<int>(self, 0x1e8) != 0) {
                GameRecord_load(F<void *>(self, 0x1e8));
                Globals_playMusicAndFadeOutCurrent(**g_tbRecordTrack);
                F<uint8_t>(self, 0x54) = 0;
                return MGame_endRunModule(F<int>(self, 0x8), 5);
            }
            Globals_playMusicAndFadeOutCurrent(**g_tbMenuTrack);
            F<uint8_t>(self, 0x54) = 0;
            ApplicationManager_SetCurrentApplicationModule(F<unsigned>(self, 0x8));
            return;
        }
    } else {
        // Modal/overlay routing while in active flight UI.
        if (F<uint8_t>(self, 0xc7) != 0) {
            Layout *hl = **g_tbStarLayout;
            if (*(uint8_t *)hl != 0) {
                Layout_OnTouchBegin((int)hl, p1);
                return;
            }
            uint8_t r = StarMap_OnTouchBegin(F<int>(self, 0x90), p1);
            F<uint8_t>(self, 0xc7) = r ^ 1;
            return;
        }
        if (F<uint8_t>(self, 0xc5) != 0 || F<uint8_t>(self, 0xce) != 0 ||
            F<uint8_t>(self, 0xc4) != 0) {
            ChoiceWindow_OnTouchBegin(F<int>(self, 0x94), p1);
            return;
        }
        if (F<uint8_t>(self, 0x5e) != 0) {
            DialogueWindow_OnTouchBegin(F<int>(self, 0x8c), p1);
            return;
        }
        if (F<uint8_t>(self, 0x15d) != 0) {
            int ad = ApplicationManager_GetApplicationData();
            if (*(uint8_t *)((char *)ad + 5) != 0) return;
            if (*(uint8_t *)((char *)ad + 0xc) != 0) return;
            MenuTouchWindow_OnTouchBegin(F<int>(self, 0x88), p1, (void *)p2);
            if (F<uint8_t>(self, 0x15e) == 0) return;
            if (MenuTouchWindow_isShowingMessage(F<void *>(self, 0x88)) != 0) return;
            if (MenuTouchWindow_isMakingScreenshot(F<void *>(self, 0x88)) != 0) return;
            // fall through to free-cam handler.
            return MGame_handleHudTouchAction(self, p1, p2, touchId, 0);
        }
    }

    // World HUD touch: dispatch on the hud's touch result bitmask.
    unsigned hr = Hud_touchBegin(F<void *>(self, 0x74), p1, p2, touchId);
    F<int>(self, 0xf8) = hr;
    MGame_handleHudTouchAction(self, p1, p2, touchId, hr);
}
