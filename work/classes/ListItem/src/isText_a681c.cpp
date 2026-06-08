#include "class.h"

// ldrb.w r0,[r0,#0x44]; bx lr
extern "C" uint8_t ListItem_isText(ListItem *self) {
    return self->f_44;
}
