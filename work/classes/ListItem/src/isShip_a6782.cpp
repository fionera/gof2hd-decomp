#include "class.h"

// ldr r0,[r0,#0xc]; cmp; it ne; mov.ne r0,#1; bx lr
extern "C" bool ListItem_isShip(ListItem *self) {
    return F<int>(self, 0xc) != 0;
}
