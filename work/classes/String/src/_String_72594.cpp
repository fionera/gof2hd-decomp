#include "class.h"

__attribute__((visibility("hidden"))) extern void *g_String_vtable;
extern "C" void operator_delete__(void *p);

// AbyssEngine::String::~String() - reset vtable, free the data buffer.
extern "C" String *String_dtor(String *self)
{
    F<void *>(self, 0x0) = g_String_vtable;
    void *data = F<void *>(self, 0x4);
    if (data != 0)
        operator_delete__(data);
    F<void *>(self, 0x4) = 0;
    return self;
}
