#include "class.h"

// if (!this[0x38]) false; else this[0x30]==1
extern "C" bool ListItem_isMoveToCargoButton(ListItem *self) {
    if (self->f_38 == 0)
        return false;
    return self->f_30 == 1;
}
