#include "class.h"

// Per-second gamma-ray damage near supernova/storm orbits, keyed by station/system ids.
// Returns the float result as raw bits (the engine treats the value as float at the call site).
__attribute__((visibility("hidden"))) extern const float g_gammaTableA[5];
__attribute__((visibility("hidden"))) extern const float g_gammaTableB[5];

static inline int as_int(float f) { union { float f; int i; } u; u.f = f; return u.i; }
static inline float as_float(unsigned u) { union { unsigned u; float f; } x; x.u = u; return x.f; }

int Status::getGammaRayDamagePerSecond(int a, int b) {
    unsigned k = a - 0x6d;
    float result = as_float(0x68409917u);
    if (k < 5) {
        if (b < 0x6a) {
            if (k < 5) return as_int(g_gammaTableA[k]);
        } else if (currentCampaignMission < 0x9e) {
            if (k < 5) return as_int(g_gammaTableB[k]);
        } else if (a == 0x6d) {
            result = as_float(0x3f800000u);
        }
    }
    return as_int(result);
}
