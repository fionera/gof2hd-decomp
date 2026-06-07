#include "class.h"

// strb.w r1,[r0,#0x410]; bx lr
extern "C" void Layout_enableFillScreen(Layout *self, bool v) {
    F<uint8_t>(self, 0x410) = v;
}
