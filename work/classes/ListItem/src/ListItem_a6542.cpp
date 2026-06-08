#include "class.h"

// ListItem::ListItem(PendingProduct*)
extern "C" void *ListItem_init(ListItem *self);

extern "C" ListItem *ListItem_ctor_PendingProduct(ListItem *self, PendingProduct *pp) {
    ListItem_init(self);
    self->f_24 = 1;
    self->f_18 = pp;
    return self;
}
