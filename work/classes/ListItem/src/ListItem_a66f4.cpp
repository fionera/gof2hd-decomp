#include "class.h"

// ListItem::ListItem(int, int) -> 0x28 = second arg, 0x2c = first arg, selectable=1
extern "C" void *ListItem_init(ListItem *self);

extern "C" ListItem *ListItem_ctor_int_int(ListItem *self, int a, int b) {
    ListItem_init(self);
    self->f_28 = b;
    self->f_2c = a;
    self->f_24 = 1;
    return self;
}
