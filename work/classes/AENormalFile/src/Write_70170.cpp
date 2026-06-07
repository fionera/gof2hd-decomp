#include "class.h"

uint32_t AENormalFile::Write(uint32_t bytes, const void *buffer)
{
    FI *h = fi_held(this);
    if (h != 0) {
        return h->vtable->Write(h, bytes, buffer);
    }
    return 0;
}
