#include "class.h"

// AbyssEngine::operator+(float const&, AbyssEngine::String const&)
// Render a float to a string, then append the rhs string.
namespace AbyssEngine {

extern "C" void String_fromFloat(String *self, float v);
extern "C" void String_appendEq(String *self, const String *o);
extern "C" void String_copy(String *self, const String *src, bool b);
extern "C" void String_dtor(String *self);

String operator+(const float &a, const String &b)
{
    String acc;
    String_fromFloat(&acc, a);
    String_appendEq(&acc, &b);

    String result;
    String_copy(&result, &acc, false);
    String_dtor(&acc);
    return result;
}

} // namespace AbyssEngine
