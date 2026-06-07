#include "class.h"

// ListItem::ListItem(Mission*)
extern "C" void *ListItem_init(ListItem *self);

extern "C" ListItem *ListItem_ctor_Mission(ListItem *self, Mission *m) {
    ListItem_init(self);
    F<uint8_t>(self, 0x24) = 1;
    F<Mission *>(self, 0x14) = m;
    return self;
}
