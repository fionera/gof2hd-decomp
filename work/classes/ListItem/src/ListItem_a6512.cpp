#include "class.h"

// ListItem::ListItem(Ship*)
extern "C" void *ListItem_init(ListItem *self);

extern "C" ListItem *ListItem_ctor_Ship(ListItem *self, Ship *s) {
    ListItem_init(self);
    self->f_24 = 1;
    self->f_c = s;
    return self;
}
