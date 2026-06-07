#include "class.h"

uint32_t AENormalFile::Read(uint32_t bytes, void *buffer)
{
    FI *h = fi_held(this);
    if (h != 0) {
        return h->vtable->Read(h, bytes, buffer);
    }
    return 0;
}
