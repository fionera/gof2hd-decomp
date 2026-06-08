#include "class.h"

extern "C" void *ListItem_init(ListItem *self);

// ListItem::ListItem(int p1, int p2, String* p3): 0x34=p1, new String(p3), 0x28=p2,
// 0x1c=s, selectable=1.
extern "C" ListItem *ListItem_ctor_int_int_String(ListItem *self, int a, int b, const void *src) {
    ListItem_init(self);
    F<int>(self, 0x34) = a;
    EngString *s = new EngString(src, false);
    F<void *>(self, 0x1c) = s;
    F<int>(self, 0x28) = b;
    F<uint8_t>(self, 0x24) = 1;
    return self;
}
