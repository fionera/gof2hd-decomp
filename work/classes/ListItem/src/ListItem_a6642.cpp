#include "class.h"

extern "C" void *ListItem_init(ListItem *self);

// ListItem::ListItem(String*, bool) -> 0x44=bool, 0x1c=new String, selectable=0
extern "C" ListItem *ListItem_ctor_String_bool(ListItem *self, const void *src, bool b) {
    ListItem_init(self);
    EngString *s = new EngString(src, false);
    F<void *>(self, 0x1c) = s;
    F<uint8_t>(self, 0x44) = b;
    F<uint8_t>(self, 0x24) = 0;
    return self;
}
