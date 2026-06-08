#include "class.h"

// ListItem::ListItem(PendingProduct*)
extern "C" void *ListItem_init(ListItem *self);

extern "C" ListItem *ListItem_ctor_PendingProduct(ListItem *self, PendingProduct *pp) {
    ListItem_init(self);
    F<uint8_t>(self, 0x24) = 1;
    F<PendingProduct *>(self, 0x18) = pp;
    return self;
}
