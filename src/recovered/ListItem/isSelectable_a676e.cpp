#include "class.h"

// ldrb.w r0,[r0,#0x24]; bx lr
extern "C" uint8_t ListItem_isSelectable(ListItem *self) {
    return F<uint8_t>(self, 0x24);
}
