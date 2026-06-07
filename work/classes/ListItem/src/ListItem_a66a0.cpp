#include "class.h"

extern "C" void *ListItem_init(ListItem *self);

// ListItem::ListItem(String* p1, String* p2): 0x1c=new String(p1), 0x20=new String(p2),
// selectable=0.
extern "C" ListItem *ListItem_ctor_String_String(ListItem *self, const void *p1, const void *p2) {
    ListItem_init(self);
    EngString *s1 = new EngString(p1, false);
    F<void *>(self, 0x1c) = s1;
    EngString *s2 = new EngString(p2, false);
    F<void *>(self, 0x20) = s2;
    F<uint8_t>(self, 0x24) = 0;
    return self;
}
