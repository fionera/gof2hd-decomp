#include "class.h"

// if (!selectable && *(int*)(*(int*)(this+0x1c)+8) != 0) return this[0x38]==0; else false
extern "C" bool ListItem_isTab(ListItem *self) {
    if (self->f_24 == 0 &&
        *(int *)(self->f_1c + 8) != 0)
        return self->f_38 == 0;
    return false;
}
