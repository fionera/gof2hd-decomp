#include "class.h"

// AbyssEngine::operator+(AbyssEngine::String const&, float const&)
// Append the rendering of a float to a string.
namespace AbyssEngine {

extern "C" void String_copy(String *self, const String *src, bool b);
extern "C" void String_fromFloat(String *self, float v);
extern "C" void String_appendEq(String *self, const String *o);
extern "C" void String_dtor(String *self);

String operator+(const String &a, const float &b)
{
    String acc;
    String_copy(&acc, &a, false);

    String num;
    String_fromFloat(&num, b);
    String_appendEq(&acc, &num);
    String_dtor(&num);

    String result;
    String_copy(&result, &acc, false);
    String_dtor(&acc);
    return result;
}

} // namespace AbyssEngine
