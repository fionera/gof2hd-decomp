#include "class.h"

// ldr r1,[r0,#0x34]; movs r0,#0; cmp.w r1,#0xffffffff; it gt; mov.gt r0,#1
extern "C" bool ListItem_isImage(ListItem *self) {
    return self->f_34 < 0x80000000u;
}
