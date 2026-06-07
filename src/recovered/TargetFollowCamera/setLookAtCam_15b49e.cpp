#include "class.h"

// strb.w r1,[r0,#0x45]
extern "C" void TFC_setLookAtCam(TargetFollowCamera *self, bool v) {
    F<uint8_t>(self, 0x45) = v;
}
