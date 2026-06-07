#include "class.h"

extern "C" void *ListItem_init(ListItem *self);

// ListItem::ListItem(ListItem* src) — copy constructor. Pointer/value fields are
// bulk-copied (16-byte NEON for 0x4..0x14 and 0x28..0x38); the two owned Strings
// at 0x1c/0x20 are deep-cloned when present.
extern "C" ListItem *ListItem_ctor_copy(ListItem *self, ListItem *src) {
    ListItem_init(self);
    F<Blk16>(self, 0x4) = F<Blk16>(src, 0x4);
    F<int>(self, 0x14) = F<int>(src, 0x14);
    F<uint8_t>(self, 0x24) = F<uint8_t>(src, 0x24);

    void *p = F<void *>(src, 0x1c);
    if (p)
        F<void *>(self, 0x1c) = new EngString(p, false);
    else
        F<void *>(self, 0x1c) = 0;
    p = F<void *>(src, 0x20);
    if (p)
        F<void *>(self, 0x20) = new EngString(p, false);
    else
        F<void *>(self, 0x20) = 0;

    F<Blk16>(self, 0x28) = F<Blk16>(src, 0x28);
    F<uint8_t>(self, 0x38) = F<uint8_t>(src, 0x38);
    F<int>(self, 0x3c) = F<int>(src, 0x3c);
    F<int>(self, 0x40) = F<int>(src, 0x40);
    F<int>(self, 0x18) = F<int>(src, 0x18);
    return self;
}
