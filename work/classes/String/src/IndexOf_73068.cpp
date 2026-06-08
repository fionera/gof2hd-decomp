#include "class.h"

// IndexOf(uint start, String const&) - the 3-arg overload at 0x73004.
extern "C" unsigned int String_IndexOf_from(String *self, unsigned int start, String *needle);

// AbyssEngine::String::IndexOf(AbyssEngine::String const&) - search from offset 0.
extern "C" unsigned int String_IndexOf(String *self, String *needle)
{
    return String_IndexOf_from(self, 0, needle);
}
