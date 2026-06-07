#include "class.h"

uint32_t AEPakFile::Read(uint32_t bytes, void *buffer)
{
    FI *h;
    if (bytes != 0 && (h = pak_fi(this)) != 0) {
        int pos = pak_pos(this);
        int size = pak_size(this);
        if ((int)(pos + bytes) > size) {
            bytes = (uint32_t)(size - pos);
        }
        if (bytes != 0) {
            pak_pos(this) = pos + bytes;
            return h->vtable->Read(h, bytes, buffer);
        }
    }
    return 0;
}
