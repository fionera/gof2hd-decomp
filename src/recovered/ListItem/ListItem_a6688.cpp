#include "class.h"

// ListItem::ListItem(int)  -> field 0x28
extern "C" void *ListItem_init(ListItem *self);

extern "C" ListItem *ListItem_ctor_int(ListItem *self, int v) {
    ListItem_init(self);
    F<uint8_t>(self, 0x24) = 1;
    F<int>(self, 0x28) = v;
    return self;
}
