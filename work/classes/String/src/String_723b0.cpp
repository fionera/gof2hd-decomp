#include "class.h"

__attribute__((visibility("hidden"))) extern void *g_String_vtable;

extern "C" {
void String_Set_wchar(String *self, const uint16_t *s);
void String_Reverse(String *self);
}

// AbyssEngine::String::String(unsigned short const*, bool reverse)
extern "C" String *String_ctor_wchar(String *self, const uint16_t *s, bool reverse)
{
    F<void *>(self, 0x0) = g_String_vtable;
    F<void *>(self, 0x4) = 0;
    String_Set_wchar(self, s);
    if (reverse)
        String_Reverse(self);
    return self;
}
