#include "class.h"

extern "C" void *ListItem_init(ListItem *self);

// ListItem::ListItem(String*, int) -> 0x30=int, 0x1c=new String, selectable=0
extern "C" ListItem *ListItem_ctor_String_int(ListItem *self, const void *src, int v) {
    ListItem_init(self);
    EngString *s = new EngString(src, false);
    self->f_1c = s;
    self->f_30 = v;
    self->f_24 = 0;
    return self;
}
