#include "class.h"

extern "C" bool ListItem_isPendingProduct(ListItem *self) {
    return F<int>(self, 0x18) != 0;
}
