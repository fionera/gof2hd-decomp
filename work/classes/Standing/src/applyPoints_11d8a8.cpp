#include "class.h"

// Standing::applyPoints(int race, int delta): add delta to standings[race] and
// clamp the result into [-100, 100]. The clamping store only runs when the value
// leaves the band; otherwise it returns early.
extern "C" void Standing_applyPoints(Standing *self, int race, int delta) {
    int *p = F<int *>(self, 0);
    int v = delta + p[race];
    p[race] = v;
    int c;
    if (v > 0x64) {
        c = 100;
    } else {
        if (v >= -0x64) return;
        c = -100;
    }
    p[race] = c;
}
