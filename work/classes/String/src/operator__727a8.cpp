#include "class.h"

extern "C" void String_Set_wchar(String *self, const uint16_t *s);

// AbyssEngine::String::operator=(AbyssEngine::String const&)
extern "C" String *String_assign(String *self, String *other)
{
    uint16_t *data = F<uint16_t *>(other, 0x4);
    if (data != 0)
        String_Set_wchar(self, data);
    return self;
}
