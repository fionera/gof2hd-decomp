#include "class.h"

extern "C" void *ListItem_init(ListItem *self);

// ListItem::ListItem(ListItem* src) — copy constructor. Pointer/value fields are
// bulk-copied (16-byte NEON for 0x4..0x14 and 0x28..0x38); the two owned Strings
// at 0x1c/0x20 are deep-cloned when present.
extern "C" ListItem *ListItem_ctor_copy(ListItem *self, ListItem *src) {
    ListItem_init(self);
    self->f_4 = src->f_4;
    self->f_14 = src->f_14;
    self->f_24 = src->f_24;

    void *p = src->f_1c;
    if (p)
        self->f_1c = new EngString(p, false);
    else
        self->f_1c = 0;
    p = src->f_20;
    if (p)
        self->f_20 = new EngString(p, false);
    else
        self->f_20 = 0;

    self->f_28 = src->f_28;
    self->f_38 = src->f_38;
    self->f_3c = src->f_3c;
    self->f_40 = src->f_40;
    self->f_18 = src->f_18;
    return self;
}
