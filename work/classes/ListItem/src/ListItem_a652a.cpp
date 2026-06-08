#include "class.h"

// ListItem::ListItem(Item*)
extern "C" void *ListItem_init(ListItem *self);

extern "C" ListItem *ListItem_ctor_Item(ListItem *self, Item *it) {
    ListItem_init(self);
    self->f_24 = 1;
    self->f_10 = it;
    return self;
}
