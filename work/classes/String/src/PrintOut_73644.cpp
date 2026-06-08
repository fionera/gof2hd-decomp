#include "class.h"

extern "C" {
char *String_GetAEChar(String *self);   // 0x6ef68 - allocates an 8-bit copy
void String_printImpl(const char *s);    // 0x1ab108 - tail-called print helper
}

// AbyssEngine::String::PrintOut() - print the string via the platform helper.
extern "C" void String_PrintOut(String *self)
{
    char *bytes = String_GetAEChar(self);
    String_printImpl(bytes);
}
