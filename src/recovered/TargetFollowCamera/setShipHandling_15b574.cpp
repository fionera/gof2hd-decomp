#include "class.h"

// Maps a 0..1 handling value to two damping coefficients (fused multiply-adds),
// stores the raw value at 0x134, then tail-calls the update helper (this, 1.0f).
extern "C" void TFC_tail_int(TargetFollowCamera *self, int n);

extern "C" void TFC_setShipHandling(TargetFollowCamera *self, float v) {
    float s = v * 0.01f;
    F<float>(self, 0x134) = v;
    F<float>(self, 0x128) = 0.003f + (1.0f - s) * 0.015f;
    F<float>(self, 0x12c) = 0.001f + s * 0.010986f;
    return TFC_tail_int(self, 0x3f800000);
}
