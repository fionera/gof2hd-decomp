#include "class.h"

// ListItem::ListItem(Array<String*>*)  -> field 0x00, selectable=0
extern "C" void *ListItem_init(ListItem *self);

extern "C" ListItem *ListItem_ctor_Array(ListItem *self, Array<String12 *> *arr) {
    ListItem_init(self);
    self->f_24 = 0;
    F<Array<String12 *> *>(self, 0x0) = arr;
    return self;
}
