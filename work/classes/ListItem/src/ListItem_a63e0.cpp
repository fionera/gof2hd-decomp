#include "class.h"

extern "C" void *ListItem_init(ListItem *self);

// ListItem::ListItem(int p1, int p2, String* p3): 0x34=p1, new String(p3), 0x28=p2,
// 0x1c=s, selectable=1.
extern "C" ListItem *ListItem_ctor_int_int_String(ListItem *self, int a, int b, const void *src) {
    ListItem_init(self);
    self->f_34 = a;
    EngString *s = new EngString(src, false);
    self->f_1c = s;
    self->f_28 = b;
    self->f_24 = 1;
    return self;
}
