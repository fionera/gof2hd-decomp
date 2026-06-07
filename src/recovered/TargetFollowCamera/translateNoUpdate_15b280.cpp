#include "class.h"

// Adds (dx,dy,dz) to both the position Vector at +0x8 and the target Vector at +0x14.
extern "C" void TFC_translateNoUpdate(TargetFollowCamera *self, float dx, float dy, float dz) {
    F<float>(self, 0x08) = F<float>(self, 0x08) + dx;
    F<float>(self, 0x0c) = F<float>(self, 0x0c) + dy;
    F<float>(self, 0x10) = F<float>(self, 0x10) + dz;
    F<float>(self, 0x14) = F<float>(self, 0x14) + dx;
    F<float>(self, 0x18) = F<float>(self, 0x18) + dy;
    F<float>(self, 0x1c) = F<float>(self, 0x1c) + dz;
}
