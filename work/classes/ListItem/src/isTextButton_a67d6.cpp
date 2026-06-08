#include "class.h"

// ldrb.w r0,[r0,#0x38]; bx lr
extern "C" uint8_t ListItem_isTextButton(ListItem *self) {
    return F<uint8_t>(self, 0x38);
}
