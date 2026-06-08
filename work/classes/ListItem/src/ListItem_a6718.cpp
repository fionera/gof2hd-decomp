#include "class.h"

// ListItem::ListItem(Mission*)
extern "C" void *ListItem_init(ListItem *self);

extern "C" ListItem *ListItem_ctor_Mission(ListItem *self, Mission *m) {
    ListItem_init(self);
    self->f_24 = 1;
    self->f_14 = m;
    return self;
}
