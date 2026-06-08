#include "class.h"

// ListItem::ListItem(String*) -> new String at 0x1c, selectable=0
extern "C" void *ListItem_init(ListItem *self);

extern "C" ListItem *ListItem_ctor_String(ListItem *self, const void *src) {
    ListItem_init(self);
    EngString *s = new EngString(src, false);
    self->f_1c = s;
    self->f_24 = 0;
    return self;
}
