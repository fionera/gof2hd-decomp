#include "class.h"

uint32_t AEPakFile::Skip(uint32_t bytes)
{
    void *buffer = operator new[](bytes);
    PakObj *self = (PakObj *)this;
    self->vtable->ReadInto(this, bytes, buffer);
    operator delete[](buffer);
    return 1;
}
