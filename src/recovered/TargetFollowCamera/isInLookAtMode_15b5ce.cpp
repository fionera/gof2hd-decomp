#include "class.h"

// ldrb.w r0,[r0,#0x45]
extern "C" uint8_t TFC_isInLookAtMode(TargetFollowCamera *self) {
    return F<uint8_t>(self, 0x45);
}
