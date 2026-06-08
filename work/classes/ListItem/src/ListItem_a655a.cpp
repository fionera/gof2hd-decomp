#include "class.h"

// ListItem::ListItem(Agent*)
extern "C" void *ListItem_init(ListItem *self);

extern "C" ListItem *ListItem_ctor_Agent(ListItem *self, Agent *a) {
    ListItem_init(self);
    F<uint8_t>(self, 0x24) = 1;
    F<Agent *>(self, 0x4) = a;
    return self;
}
