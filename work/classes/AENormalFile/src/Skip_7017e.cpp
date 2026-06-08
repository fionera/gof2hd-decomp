#include "class.h"

uint32_t AENormalFile::Skip(uint32_t bytes)
{
    FI *h = this->held;
    if (h != 0) {
        return h->vtable->Skip(h, bytes);
    }
    return 0;
}
