#include "class.h"

// ListItem::ListItem(Item*)
extern "C" void *ListItem_init(ListItem *self);

extern "C" ListItem *ListItem_ctor_Item(ListItem *self, Item *it) {
    ListItem_init(self);
    F<uint8_t>(self, 0x24) = 1;
    F<Item *>(self, 0x10) = it;
    return self;
}
