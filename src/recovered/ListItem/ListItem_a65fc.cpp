#include "class.h"

extern "C" void *ListItem_init(ListItem *self);

// ListItem::ListItem(String*, int) -> 0x30=int, 0x1c=new String, selectable=0
extern "C" ListItem *ListItem_ctor_String_int(ListItem *self, const void *src, int v) {
    ListItem_init(self);
    EngString *s = new EngString(src, false);
    F<void *>(self, 0x1c) = s;
    F<int>(self, 0x30) = v;
    F<uint8_t>(self, 0x24) = 0;
    return self;
}
