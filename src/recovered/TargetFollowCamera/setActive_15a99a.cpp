#include "class.h"

// strb.w r1,[r0,#0x46]
extern "C" void TFC_setActive(TargetFollowCamera *self, bool v) {
    F<uint8_t>(self, 0x46) = v;
}
