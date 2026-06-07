#include "class.h"

// str.w r1,[r0,#0x3b0]; bx lr
extern "C" void Layout_setDrawColor(Layout *self, int color) {
    F<int>(self, 0x3b0) = color;
}
