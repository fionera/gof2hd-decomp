#include "class.h"

extern "C" void *ListItem_init(ListItem *self);

// ListItem::ListItem(String*, bool) -> 0x44=bool, 0x1c=new String, selectable=0
extern "C" ListItem *ListItem_ctor_String_bool(ListItem *self, const void *src, bool b) {
    ListItem_init(self);
    EngString *s = new EngString(src, false);
    self->f_1c = s;
    self->f_44 = b;
    self->f_24 = 0;
    return self;
}
