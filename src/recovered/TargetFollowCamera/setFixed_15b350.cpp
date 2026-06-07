#include "class.h"

// strb.w r1,[r0,#0x138]
extern "C" void TFC_setFixed(TargetFollowCamera *self, bool v) {
    F<uint8_t>(self, 0x138) = v;
}
