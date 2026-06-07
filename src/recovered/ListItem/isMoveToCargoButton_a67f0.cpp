#include "class.h"

// if (!this[0x38]) false; else this[0x30]==1
extern "C" bool ListItem_isMoveToCargoButton(ListItem *self) {
    if (F<uint8_t>(self, 0x38) == 0)
        return false;
    return F<int>(self, 0x30) == 1;
}
