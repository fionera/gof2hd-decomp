#include "class.h"

// ListItem::ListItem(int)  -> field 0x28
extern "C" void *ListItem_init(ListItem *self);

extern "C" ListItem *ListItem_ctor_int(ListItem *self, int v) {
    ListItem_init(self);
    self->f_24 = 1;
    self->f_28 = v;
    return self;
}
