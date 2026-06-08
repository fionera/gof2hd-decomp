#include "class.h"

// AbyssEngine::operator+(AbyssEngine::String const&)
// Unary form: construct a (non-owning) copy of the operand string.
namespace AbyssEngine {

extern "C" void String_copy(String *self, const String *src, bool b);

String operator+(const String &a)
{
    String result;
    String_copy(&result, &a, false);
    return result;
}

} // namespace AbyssEngine
