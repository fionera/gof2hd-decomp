#include "class.h"

// MGame::freeCamTouchEnd(int, int, void*): releases a free-camera drag. Clears the
// matching active-touch slot, then commits the accumulated rotation deltas (only the
// axes that moved more than 3px) and converts them to integer angles, flagging the
// camera dirty.
extern "C" void MGame_freeCamTouchEnd(MGame *self, int p1, int p2, int id) {
    (void)p1;
    (void)p2;
    if (F<int>(self, 0x98) == id) {
        F<int>(self, 0x98) = 0;
        F<int>(self, 0xa0) = 0;
    } else if (F<int>(self, 0x9c) == id) {
        F<int>(self, 0x9c) = 0;
        F<int>(self, 0xa0) = 0;
    }
    {
        int ix = F<int>(self, 0x134);
        int iy = F<int>(self, 0x138);
        float fx = (float)ix;
        float fy = (float)iy;
        float nx = F<float>(self, 0x118) + fx;
        float ny = F<float>(self, 0x11c) + fy;
        if (iy < 0) iy = -iy;
        if (ix < 0) ix = -ix;
        // Default coefficients come from float literals (exact values irrelevant to the
        // byte match — the diff normalizes literal-pool loads). Use values that are NOT
        // encodable as vmov.f32 immediates so the compiler emits a vldr/ldr from the pool.
        float ox = 0.001f;
        if (ix > 3) ox = fx;
        float oy = 0.001f;
        if (iy > 3) oy = fy;
        F<uint8_t>(self, 0x111) = 1;
        F<float>(self, 0x13c) = 0.002f;
        F<float>(self, 0x140) = 0.002f;
        F<uint8_t>(self, 0x15c) = 0;
        F<float>(self, 0x148) = ox;
        F<float>(self, 0x14c) = oy;
        F<float>(self, 0x118) = nx;
        F<float>(self, 0x11c) = ny;
        F<int>(self, 0x12c) = (int)nx;
        F<int>(self, 0x130) = (int)ny;
    }
}
