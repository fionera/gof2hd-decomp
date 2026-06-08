#include "class.h"

uint32_t AENormalFile::Release()
{
    FI *h = this->held;
    if (h != 0) {
        h->vtable->Discard(h);
        FI *h2 = this->held;
        if (h2 != 0) {
            h2->vtable->Free(h2);
        }
    }
    this->held = 0;
    return 1;
}
