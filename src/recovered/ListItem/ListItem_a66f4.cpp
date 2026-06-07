#include "class.h"

// ListItem::ListItem(int, int) -> 0x28 = second arg, 0x2c = first arg, selectable=1
extern "C" void *ListItem_init(ListItem *self);

extern "C" ListItem *ListItem_ctor_int_int(ListItem *self, int a, int b) {
    ListItem_init(self);
    F<int>(self, 0x28) = b;
    F<int>(self, 0x2c) = a;
    F<uint8_t>(self, 0x24) = 1;
    return self;
}
