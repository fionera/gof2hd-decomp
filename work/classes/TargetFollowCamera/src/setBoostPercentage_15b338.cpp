#include "class.h"

// Clamp n to [2,8], store (pct, n) at 0x110/0x114.
extern "C" void TFC_setBoostPercentage(TargetFollowCamera *self, float pct, int n) {
    if (n > 8) n = 8;
    if (n < 2) n = 2;
    F<float>(self, 0x110) = pct;
    F<int>(self, 0x114) = n;
}
