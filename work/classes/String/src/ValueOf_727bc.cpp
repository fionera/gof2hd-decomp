#include "class.h"

extern "C" {
char *String_GetAEChar(String *self);   // 0x6ef68 - allocates an 8-bit copy
int atoi(const char *s);                 // 0x6f5a4
void operator_delete__(void *p);
}

// AbyssEngine::String::ValueOf() - parse the string as an integer.
extern "C" int String_ValueOf(String *self)
{
    char *bytes = String_GetAEChar(self);
    int v = atoi(bytes);
    operator_delete__(bytes);
    return v;
}
