#include "class.h"

// ldrb.w r0,[r0,#0x4d]
extern "C" uint8_t TFC_isInFastForwardMode(TargetFollowCamera *self) {
    return F<uint8_t>(self, 0x4d);
}
