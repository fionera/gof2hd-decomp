#include "class.h"

uint32_t AENormalFile::Skip(uint32_t bytes)
{
    FI *h = fi_held(this);
    if (h != 0) {
        return h->vtable->Skip(h, bytes);
    }
    return 0;
}
