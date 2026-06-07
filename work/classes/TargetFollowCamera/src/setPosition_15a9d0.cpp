#include "class.h"

// adds r0,#8 ; stmia r0!,{r1,r2,r3} ; bx lr
// Writes the position Vector at +0x8 and returns &field_14 (this+0x14): the
// store base register is post-incremented and reused as the return value.
extern "C" float *TFC_setPosition3(float *p, float x, float y, float z) {
    p += 2;          // &field at +0x8
    *p++ = x;
    *p++ = y;
    *p++ = z;
    return p;
}
