#include "class.h"

extern "C" void *ListItem_init(ListItem *self);

// ListItem::ListItem(String*, bool p2, int p3): 0x38=1, 0x1c=new String, 0x30=p3,
// selectable=p2.
extern "C" ListItem *ListItem_ctor_String_bool_int(ListItem *self, const void *src, bool b, int v) {
    ListItem_init(self);
    EngString *s = new EngString(src, false);
    F<uint8_t>(self, 0x38) = 1;
    F<void *>(self, 0x1c) = s;
    F<int>(self, 0x30) = v;
    F<uint8_t>(self, 0x24) = b;
    return self;
}
