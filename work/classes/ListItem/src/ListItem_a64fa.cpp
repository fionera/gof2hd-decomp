#include "class.h"

// ListItem::ListItem(BluePrint*)
extern "C" void *ListItem_init(ListItem *self);

extern "C" ListItem *ListItem_ctor_BluePrint(ListItem *self, BluePrint *bp) {
    ListItem_init(self);
    F<uint8_t>(self, 0x24) = 1;
    F<BluePrint *>(self, 0x8) = bp;
    return self;
}
