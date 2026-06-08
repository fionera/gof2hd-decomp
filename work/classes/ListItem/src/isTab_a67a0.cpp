#include "class.h"

// if (!selectable && *(int*)(*(int*)(this+0x1c)+8) != 0) return this[0x38]==0; else false
extern "C" bool ListItem_isTab(ListItem *self) {
    if (F<uint8_t>(self, 0x24) == 0 &&
        *(int *)(F<int>(self, 0x1c) + 8) != 0)
        return F<uint8_t>(self, 0x38) == 0;
    return false;
}
