#include "class.h"

// if (!this[0x38]) false; else this[0x30]==0
extern "C" bool ListItem_isSellButton(ListItem *self) {
    if (self->f_38 == 0)
        return false;
    return self->f_30 == 0;
}
