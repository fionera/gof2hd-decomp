#include "class.h"

// AbyssEngine::operator+(AbyssEngine::String const&, AbyssEngine::String const&)
// Concatenate two strings: copy lhs into a temp, append rhs, return the temp.
namespace AbyssEngine {

extern "C" void String_copy(String *self, const String *src, bool b);   // String(String const&, bool)
extern "C" void String_appendEq(String *self, const String *o);          // operator+=
extern "C" void String_dtor(String *self);                               // ~String

String operator+(const String &a, const String &b)
{
    String tmp;
    String_copy(&tmp, &a, false);
    String_appendEq(&tmp, &b);

    String result;
    String_copy(&result, &tmp, false);
    String_dtor(&tmp);
    return result;
}

} // namespace AbyssEngine
