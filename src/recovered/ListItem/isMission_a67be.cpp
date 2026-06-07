#include "class.h"

extern "C" bool ListItem_isMission(ListItem *self) {
    return F<int>(self, 0x14) != 0;
}
