#include "class.h"

extern "C" bool ListItem_isSlot(ListItem *self) {
    return F<uint32_t>(self, 0x28) < 0x80000000u;
}
