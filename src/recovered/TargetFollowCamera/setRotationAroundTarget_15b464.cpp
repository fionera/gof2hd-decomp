#include "class.h"

// strb.w r1,[r0,#0x4c]
extern "C" void TFC_setRotationAroundTarget(TargetFollowCamera *self, bool v) {
    F<uint8_t>(self, 0x4c) = v;
}
