#include "class.h"

struct TargetFollowCamera;
struct Hud;
// Hidden PC-relative globals: a "cinematic active" byte flag and an input/UI byte flag.
__attribute__((visibility("hidden"))) extern uint8_t *g_cinFlagA;
__attribute__((visibility("hidden"))) extern uint8_t **g_cinFlagB;

extern "C" void MGame_switchCamera(MGame *self, int id);
extern "C" uint8_t TFC_isInLookAtMode(TargetFollowCamera *c);
extern "C" void TFC_setLookAtCam(TargetFollowCamera *c, int v);
// camera enter/leave cinematic helper (this->[0xf4], byte[0x164]) — tail call.
extern "C" void Cam_setCinematic(TargetFollowCamera *c, int on);
// hud cinematic helper (**[0x78], [0x40], 1) — tail call.
extern "C" void Hud_enterCinematic(Hud *h, int a, int one);

// MGame::setCinematicMode(bool): toggles cinematic camera/UI state, switching cameras
// and (de)activating the look-at HUD overlay.
extern "C" void MGame_setCinematicMode(MGame *self, bool on) {
    F<uint8_t>(self, 0x15e) = on;
    *g_cinFlagA = on;
    uint8_t *flag = *g_cinFlagB;
    if (!on) {
        *flag = F<uint8_t>(self, 0x1dd);
        MGame_switchCamera(self, F<int>(self, 0x160));
        return Cam_setCinematic(F<TargetFollowCamera *>(self, 0xf4), F<uint8_t>(self, 0x164));
    }
    F<uint8_t>(self, 0x1dd) = *flag;
    *flag = 1;
    if (F<uint8_t>(self, 0xdc) == 0 && F<uint8_t>(self, 0x5f) == 0) {
        F<int>(self, 0x160) = F<int>(self, 0x14);
        F<uint8_t>(self, 0x164) = TFC_isInLookAtMode(F<TargetFollowCamera *>(self, 0xf4));
        TFC_setLookAtCam(F<TargetFollowCamera *>(self, 0xf4), 0);
        MGame_switchCamera(self, 3);
        return Hud_enterCinematic(*F<Hud **>(self, 0x78), F<int>(self, 0x40), 1);
    }
}
