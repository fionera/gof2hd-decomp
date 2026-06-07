#include "class.h"

// Same as translateNoUpdate, then tail-calls the shake/update helper (this, 1000).
extern "C" void TFC_tail_int(TargetFollowCamera *self, int n);

extern "C" void TFC_translate(TargetFollowCamera *self, float dx, float dy, float dz) {
    F<float>(self, 0x08) = F<float>(self, 0x08) + dx;
    F<float>(self, 0x0c) = F<float>(self, 0x0c) + dy;
    F<float>(self, 0x10) = F<float>(self, 0x10) + dz;
    F<float>(self, 0x14) = F<float>(self, 0x14) + dx;
    F<float>(self, 0x18) = F<float>(self, 0x18) + dy;
    F<float>(self, 0x1c) = F<float>(self, 0x1c) + dz;
    return TFC_tail_int(self, 1000);
}
