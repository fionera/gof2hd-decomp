#include "class.h"

extern "C" void *ListItem_init(ListItem *self);

// ListItem::ListItem(String*, bool p2, int p3): 0x38=1, 0x1c=new String, 0x30=p3,
// selectable=p2.
extern "C" ListItem *ListItem_ctor_String_bool_int(ListItem *self, const void *src, bool b, int v) {
    ListItem_init(self);
    EngString *s = new EngString(src, false);
    self->f_38 = 1;
    self->f_1c = s;
    self->f_30 = v;
    self->f_24 = b;
    return self;
}
