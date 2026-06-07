#include "class.h"

// ldrb.w r0,[r0,#0x3c0]; bx lr
extern "C" uint8_t Layout_helpPressed(Layout *self) {
    return F<uint8_t>(self, 0x3c0);
}
