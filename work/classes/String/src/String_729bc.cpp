#include "class.h"

__attribute__((visibility("hidden"))) extern void *g_String_vtable;

extern "C" void String_Set_float(String *self, float v);

// AbyssEngine::String::String(float) - formatted string of a float.
extern "C" String *String_ctor_float(String *self, float v)
{
    F<void *>(self, 0x0) = g_String_vtable;
    F<void *>(self, 0x4) = 0;
    String_Set_float(self, v);
    return self;
}
