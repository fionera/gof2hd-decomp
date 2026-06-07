#include "class.h"

extern "C" bool ListItem_isBluePrint(ListItem *self) {
    return F<int>(self, 0x8) != 0;
}
