#include "class.h"

__attribute__((visibility("hidden"))) extern int **gFmod;  // ldr [0xe4d70]
// FModSound::play(int sound, Vector* a, Vector* b, float v) — blx 0x71548.
struct Vec3 { float x, y, z; };
extern "C" void FModSound_play(int sound, Vec3 *a, Vec3 *b, float v);

// Layout::showMissionRewardMessage(int show, bool flag)
extern "C" void Layout_showMissionRewardMessage(Layout *self, int show, bool flag) {
    if (show == 0)
        return;
    F<uint8_t>(self, 0x2ed) = flag;
    F<uint8_t>(self, 0x2ec) = 1;
    int *g = *gFmod;
    F<int>(self, 0x3d0) = 0;
    F<int>(self, 0x3d4) = show;
    FModSound_play(*g, (Vec3 *)0x24, 0, 0.0f);
}
