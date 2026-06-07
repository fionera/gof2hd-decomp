#include "class.h"

uint32_t AENormalFile::Release()
{
    FI *h = fi_held(this);
    if (h != 0) {
        h->vtable->Discard(h);
        FI *h2 = fi_held(this);
        if (h2 != 0) {
            h2->vtable->Free(h2);
        }
    }
    fi_held(this) = 0;
    return 1;
}
