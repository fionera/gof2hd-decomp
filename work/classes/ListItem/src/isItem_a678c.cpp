#include "class.h"

extern "C" bool ListItem_isItem(ListItem *self) {
    return F<int>(self, 0x10) != 0;
}
