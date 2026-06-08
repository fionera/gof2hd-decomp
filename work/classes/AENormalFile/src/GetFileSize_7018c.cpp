#include "class.h"

uint32_t AENormalFile::GetFileSize()
{
    FI *h = fi_held(this);
    if (h != 0) {
        return h->vtable->GetFileSize(h);
    }
    return 0;
}
