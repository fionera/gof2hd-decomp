#include "class.h"

// ListItem::ListItem(Ship*)
extern "C" void *ListItem_init(ListItem *self);

extern "C" ListItem *ListItem_ctor_Ship(ListItem *self, Ship *s) {
    ListItem_init(self);
    F<uint8_t>(self, 0x24) = 1;
    F<Ship *>(self, 0xc) = s;
    return self;
}
