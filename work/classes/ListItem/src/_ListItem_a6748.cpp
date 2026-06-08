#include "class.h"

// ListItem::~ListItem() — for each of the two owned String* (at 0x1c, 0x20):
// if non-null, call its virtual function at vtable+4 (deleting dtor) then null it.
// Authored as a real C++ destructor so it demangles to ListItem::~ListItem().
ListItem::~ListItem() {
    void **p = F<void **>(this, 0x1c);
    if (p) {
        (*(void (**)(void *))(*(char **)p + 4))(p);
        F<void *>(this, 0x1c) = 0;
    }
    p = F<void **>(this, 0x20);
    if (p) {
        (*(void (**)(void *))(*(char **)p + 4))(p);
        F<void *>(this, 0x20) = 0;
    }
}
