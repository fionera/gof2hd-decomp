#include "class.h"

__attribute__((visibility("hidden"))) extern void *g_String_vtable;

extern "C" {
void String_Set_wchar(String *self, const uint16_t *s);
void String_Reverse(String *self);
}

// AbyssEngine::String::String(AbyssEngine::String const&, bool reverse) - copy ctor.
extern "C" String *String_ctor_copy(String *self, String *other, bool reverse)
{
    F<void *>(self, 0x0) = g_String_vtable;
    F<void *>(self, 0x4) = 0;
    String_Set_wchar(self, F<uint16_t *>(other, 0x4));
    if (reverse)
        String_Reverse(self);
    return self;
}
