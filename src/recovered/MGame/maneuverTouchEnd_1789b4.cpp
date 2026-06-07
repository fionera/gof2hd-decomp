#include "class.h"

struct PlayerEgo;
__attribute__((visibility("hidden"))) extern int *g_maneuverScale;
extern "C" void PlayerEgo_initManeuver(PlayerEgo *p, int dir);

// MGame::maneuverTouchEnd(): if a maneuver gesture was active, recent, and the finger
// moved far enough, trigger a left/right maneuver on the player; always clear the flag.
extern "C" void MGame_maneuverTouchEnd(MGame *self, int a, void *p) {
    (void)p;
    if (F<uint8_t>(self, 0x17c) != 0 && F<int>(self, 0x178) <= 0x258) {
        float f2 = (float)(*g_maneuverScale);
        int d = a - F<int>(self, 0x180);
        if (d < 0) d = -d;
        float f3 = (float)d;
        if ((f2 / 320.0f) * 0.15f < f3) {
            int dir = 1;
            if (F<int>(self, 0x180) < a) dir = 2;
            PlayerEgo_initManeuver(F<PlayerEgo *>(self, 0x58), dir);
        }
    }
    F<uint8_t>(self, 0x17c) = 0;
}
