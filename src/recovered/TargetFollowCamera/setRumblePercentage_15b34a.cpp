#include "class.h"

// strd r1,r2,[r0,#0x110]
extern "C" void TFC_setRumblePercentage(TargetFollowCamera *self, float pct, int n) {
    F<float>(self, 0x110) = pct;
    F<int>(self, 0x114) = n;
}
