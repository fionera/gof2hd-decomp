#include "class.h"

// ldrb r1,[0x44]; cmp #0; itee eq; mov.eq 0; ldr.ne r0,[r0,#0]; ldr.ne r0,[r0,#0]
// if (!textFlag) return 0; else return **(int**)this  (first line count of the strings array)
extern "C" int ListItem_getNumLines(ListItem *self) {
    if (F<uint8_t>(self, 0x44) == 0)
        return 0;
    return **(int **)self;
}
