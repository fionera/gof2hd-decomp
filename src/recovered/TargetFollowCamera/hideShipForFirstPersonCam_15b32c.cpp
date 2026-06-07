#include "class.h"

// ldrb.w r0,[r0,#0x100]
extern "C" uint8_t TFC_hideShipForFirstPersonCam(TargetFollowCamera *self) {
    return F<uint8_t>(self, 0x100);
}
