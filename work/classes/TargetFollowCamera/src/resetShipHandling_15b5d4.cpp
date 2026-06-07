#include "class.h"

// Restores default ship-handling damping coefficients (0x128/0x12c) then tail-calls
// the update helper (this, 1.0f).
extern "C" void TFC_tail_int(TargetFollowCamera *self, int n);

static inline float bits(uint32_t u) {
    float f;
    __builtin_memcpy(&f, &u, 4);
    return f;
}

extern "C" void TFC_resetShipHandling(TargetFollowCamera *self) {
    F<float>(self, 0x128) = bits(0x3ba3d70a);  // ~0.005
    F<float>(self, 0x12c) = bits(0x3bc49ba6);  // ~0.006
    return TFC_tail_int(self, 0x3f800000);
}
