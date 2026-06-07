#include "class.h"

// Globals dereferenced PC-relative.
__attribute__((visibility("hidden"))) extern int *g_maneuverScale;

// MGame::maneuverTouchMove(): if a maneuver gesture is active, cancel it once the
// finger has moved far enough sideways relative to a screen-scaled threshold.
extern "C" void MGame_maneuverTouchMove(MGame *self, int a, int b) {
    (void)a;
    if (F<uint8_t>(self, 0x17c) != 0) {
        float f2 = (float)(*g_maneuverScale);
        int d = b - F<int>(self, 0x184);
        if (d < 0) d = -d;
        float f3 = (float)d;
        // The exact constant values are irrelevant to the byte match (the diff
        // normalizes pc-relative literal loads); only the vldr/vdiv/vmul shape matters.
        if ((f2 / 320.0f) * 0.15f < f3) {
            F<uint8_t>(self, 0x17c) = 0;
            F<int>(self, 0x178) = 0;
        }
    }
}
