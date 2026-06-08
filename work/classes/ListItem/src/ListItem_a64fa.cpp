#include "class.h"

// ListItem::ListItem(BluePrint*)
extern "C" void *ListItem_init(ListItem *self);

extern "C" ListItem *ListItem_ctor_BluePrint(ListItem *self, BluePrint *bp) {
    ListItem_init(self);
    self->f_24 = 1;
    self->f_8 = bp;
    return self;
}
