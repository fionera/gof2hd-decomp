#include "class.h"

// strb.w r1,[r0,#0x10c]
extern "C" void TFC_useTargetsUpVector(TargetFollowCamera *self, bool v) {
    F<uint8_t>(self, 0x10c) = v;
}
