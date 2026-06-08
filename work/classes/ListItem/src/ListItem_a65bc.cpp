#include "class.h"

// ListItem::ListItem(String*) -> new String at 0x1c, selectable=0
extern "C" void *ListItem_init(ListItem *self);

extern "C" ListItem *ListItem_ctor_String(ListItem *self, const void *src) {
    ListItem_init(self);
    EngString *s = new EngString(src, false);
    F<void *>(self, 0x1c) = s;
    F<uint8_t>(self, 0x24) = 0;
    return self;
}
