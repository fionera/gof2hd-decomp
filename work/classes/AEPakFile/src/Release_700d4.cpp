#include "class.h"

uint32_t AEPakFile::Release()
{
    FI *h = pak_fi(this);
    if (h != 0) {
        h->vtable->Discard(h);
        FI *h2 = pak_fi(this);
        if (h2 != 0) {
            h2->vtable->Free(h2);
        }
        pak_fi(this) = 0;
    }
    return 1;
}
