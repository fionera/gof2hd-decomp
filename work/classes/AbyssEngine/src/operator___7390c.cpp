#include "class.h"

// AbyssEngine::operator==(AbyssEngine::String const&, AbyssEngine::String const&)
// Copy lhs into a temp, compare against rhs, return the comparison result.
namespace AbyssEngine {

extern "C" void String_copy(String *self, const String *src, bool b);
extern "C" int String_Compare(String *self, const String *o);
extern "C" void String_dtor(String *self);

bool operator==(const String &a, const String &b)
{
    String tmp;
    String_copy(&tmp, &a, false);
    int cmp = String_Compare(&tmp, &b);
    String_dtor(&tmp);
    return cmp != 0;
}

} // namespace AbyssEngine
