#include "class.h"

// ListItem::ListItem(Agent*)
extern "C" void *ListItem_init(ListItem *self);

extern "C" ListItem *ListItem_ctor_Agent(ListItem *self, Agent *a) {
    ListItem_init(self);
    self->f_24 = 1;
    self->f_4 = a;
    return self;
}
