#include "class.h"

struct PlayerEgo;

extern "C" int MenuTouchWindow_isShowingMessage(void *w);                 // 0x781e4
extern "C" int MenuTouchWindow_isMakingScreenshot(void *w);               // 0x781f0
extern "C" void MGame_freeCamTouchMove(MGame *self, int p1, int p2, void *p3); // 0x78238
extern "C" int Hud_touchMove(void *hud, int p1, int p2, void *p3);        // 0x78244
extern "C" void MGame_maneuverTouchMove(MGame *self, int mode, int y);    // 0x78250
extern "C" float PlayerEgo_getThrust(PlayerEgo *p);                       // 0x7825c
extern "C" void PlayerEgo_setThrust(PlayerEgo *p, float v);               // 0x78268
extern "C" void PlayerEgo_throttleChanged(PlayerEgo *p);                  // 0x78274
extern "C" int ApplicationManager_GetApplicationData();                   // 0x71704
extern "C" void MenuTouchWindow_OnTouchMove(void *w, int p1, int p2, void *p3); // 0x75118
extern "C" void DialogueWindow_OnTouchMove(int w, int p1, int p2);        // 0x750d0
extern "C" void StarMap_OnTouchMove(int m, int p1, int p2);               // 0x750e8
extern "C" void Layout_OnTouchMove2(int l, int p1, int p2);              // 0x74758
extern "C" void ChoiceWindow_OnTouchMove(int w, int p1, int p2);          // 0x7474c
// Thrust-from-drag application (0x189af2..0x189b46): new = max(2, base + dY*scale),
// then setThrust + throttleChanged. Helper keeps the corrupt float math compiling.
extern "C" void applyThrust(MGame *self, int y);

__attribute__((visibility("hidden"))) extern int **g_tmShipTune;  // @0x189b12 ([0][0x2f8] thrust scale)
__attribute__((visibility("hidden"))) extern int **g_tmStarMap;   // @0x189b8a (StarMap/Layout sel)
__attribute__((visibility("hidden"))) extern int **g_tmAppData;   // @0x189bf8

// MGame::OnTouchMove(int p1, int y, void *touch)
extern "C" void MGame_OnTouchMove(MGame *self, int p1, int y, void *touch) {
    int handledFree = 0;
    if (F<uint8_t>(self, 0x5d) != 0) {
        uint8_t fc = F<uint8_t>(self, 0x15e);
        int allowFree = (fc != 0);
        if (fc != 0 &&
            MenuTouchWindow_isShowingMessage(F<void *>(self, 0x88)) == 0 &&
            MenuTouchWindow_isMakingScreenshot(F<void *>(self, 0x88)) == 0) {
            allowFree = 1;
        } else {
            allowFree = 0;
        }
        (void)allowFree;
    }

    if (F<uint8_t>(self, 0x15c) != 0 && F<int>(self, 0xf8) == 0 &&
        F<int>(self, 0x14) == 3) {
        MGame_freeCamTouchMove(self, p1, y, touch);
        handledFree = 1;
    }

    if (!handledFree) {
        if (F<uint8_t>(self, 0x5d) == 0) {
            int hh = Hud_touchMove(F<void *>(self, 0x74), p1, y, touch);
            F<int>(self, 0xf8) = hh;
            unsigned mode = (unsigned)F<int>(self, 0x14);
            if (mode <= 1) {
                MGame_maneuverTouchMove(self, mode, y);
                if (F<uint8_t>(self, 0x1b8) != 0 && F<uint8_t>(self, 0x5f) == 0) {
                    int f8 = F<int>(self, 0xf8);
                    int ok = (f8 == 0) ||
                             (f8 == 0x20 && F<void *>(self, 0xc0) != touch);
                    if (ok) {
                        float thrust;
                        if (F<uint8_t>(self, 0x1b9) == 0) {
                            float fy = (float)y;
                            float start = (float)F<int>(self, 0x1c0);
                            float thresh = (float)F<int>(self, 0x1cc);
                            float d = fy - start;
                            float ad = (d > 0.0f) ? d : -d;
                            if (ad > thresh) {
                                int dir = (start > fy) ? 1 : -1;
                                int ny = dir + y;
                                F<int>(self, 0x1c8) = 0;
                                F<int>(self, 0x1c0) = ny;
                                thrust = PlayerEgo_getThrust(F<PlayerEgo *>(self, 0x58));
                                F<int>(self, 0x1d0) = (int)thrust;
                                F<uint8_t>(self, 0x1b9) = 1;
                                applyThrust(self, y);
                            }
                        } else {
                            F<uint8_t>(self, 0x1b9) = 1;
                            applyThrust(self, y);
                        }
                    }
                }
            }
        }
    }

    if (F<uint8_t>(self, 0x5d) == 0) return;

    // Modal / overlay touch routing.
    if (F<uint8_t>(self, 0x60) != 0 || F<uint8_t>(self, 0xc5) != 0 ||
        F<uint8_t>(self, 0xce) != 0 || F<uint8_t>(self, 0xc4) != 0) {
        ChoiceWindow_OnTouchMove(F<int>(self, 0x94), p1, y);
        return;
    }
    if (F<uint8_t>(self, 0xc7) != 0) {
        int sel = **g_tmStarMap;
        if (*(uint8_t *)sel == 0)
            StarMap_OnTouchMove(F<int>(self, 0x90), p1, y);
        else
            Layout_OnTouchMove2(**g_tmStarMap, p1, y);
        return;
    }
    if (F<uint8_t>(self, 0x5e) != 0) {
        DialogueWindow_OnTouchMove(F<int>(self, 0x8c), p1, y);
        return;
    }
    if (F<uint8_t>(self, 0xc9) != 0) {
        int ad = **g_tmAppData;
        if (*(uint8_t *)((char *)ad + 5) == 0 && *(uint8_t *)((char *)ad + 0xc) == 0)
            MenuTouchWindow_OnTouchMove(F<void *>(self, 0x88), p1, y, touch);
    }
}
