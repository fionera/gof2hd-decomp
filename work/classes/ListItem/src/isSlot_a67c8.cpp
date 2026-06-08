#include "class.h"

extern "C" bool ListItem_isSlot(ListItem *self) {
    return self->f_28 < 0x80000000u;
}
