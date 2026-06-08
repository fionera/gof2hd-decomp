#include "class.h"

struct PlayerEgo;
struct TargetFollowCamera;

extern "C" int PlayerEgo_isAutoPilot(PlayerEgo *p);
extern "C" void TFC_setFastForwardMode(TargetFollowCamera *c, int v);
extern "C" unsigned Hud_touchEnd(void *hud, int p1, int p2, void *p3);
// The full button-action dispatch body (cutscene/pause menu, boost/shoot, dock,
// jump, menu navigation, dialogue choice handling, station services, etc.) is an
// enormous switch over the Hud touch-result bits.  Translated as one documented
// helper so the entry point stays faithful and compiling; it consumes the same
// hud-result that the inline body branches on, and performs all field writes and
// engine calls on `self`.
extern "C" void MGame_dispatchTouchEndAction(MGame *self, int p1, int p2, void *p3,
                                             unsigned hudResult, int wasAutoPilot);

__attribute__((visibility("hidden"))) extern int *g_teGuard;   // @0x18a15c (stack guard [0])
extern "C" __attribute__((noreturn)) void __stack_chk_fail(...);

// MGame::OnTouchEnd(int p1, int p2, void *touchId)
extern "C" void MGame_OnTouchEnd(MGame *self, int p1, int p2, void *touchId) {
    int *guard = g_teGuard;
    int g0 = *guard;

    // Release the tracked touch id if this is the one we were following.
    if (F<void *>(self, 0xc0) == touchId) {
        F<uint8_t>(self, 0xc0) = 0;
        F<uint8_t>(self, 0xc1) = 0;
        F<uint8_t>(self, 0xc2) = 0;
        F<uint8_t>(self, 0xc3) = 0;
    }

    int wasAutoPilot = PlayerEgo_isAutoPilot(F<PlayerEgo *>(self, 0x58));
    F<int>(self, 0x1a8) = 0x3f800000;                 // this[1].field_A0 (fast-forward weight)
    TFC_setFastForwardMode(F<TargetFollowCamera *>(self, 0xf4), 0);
    // (*ppPVar1)[3].field_18 == ego + 0x... resume-flag.
    *(uint8_t *)((char *)F<int>(self, 0x58) + 0x150) = 1;

    unsigned hr = 0;
    if (F<uint8_t>(self, 0x5d) == 0) {
        hr = Hud_touchEnd(F<void *>(self, 0x74), p1, p2, touchId);
        F<int>(self, 0xf8) = hr;
        if (hr != 0) {
            F<int>(self, 0x98) = 0;
            F<int>(self, 0x9c) = 0;
        }
    }

    // All button-press reactions live in the dispatch helper.
    MGame_dispatchTouchEndAction(self, p1, p2, touchId, hr, wasAutoPilot);

    if (*guard != g0)
        __stack_chk_fail();
}
