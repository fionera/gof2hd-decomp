#include "class.h"

// ldrb.w r0,[r0,#0x400]; bx lr
extern "C" uint8_t Layout_isFading(Layout *self) {
    return F<uint8_t>(self, 0x400);
}
